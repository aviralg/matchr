#include "matchr.h"
#include "matcher.h"
#include "Value.h"
#include "Context.h"
#include "Trace.h"

Context* pattern_match(pattern_t pattern, Value* value, Trace* trace);
Context* pattern_match_inner(pattern_t pattern, Value* value, Trace* trace);
Context*
match_vec(pattern_t pattern, Value* value, SEXPTYPE type, Trace* trace);
Context*
match_slice(pattern_t pattern, int pat_idx, Value* value, Trace* trace);
Context* match_range(pattern_t pattern, Value* value, Trace* trace);

SEXP r_matchr_match(SEXP r_matcher, SEXP r_value, SEXP r_trace) {
    matcher_t matcher = matcher_unwrap(r_matcher);

    int size = matcher->clauses.size();

    if (TYPEOF(r_trace) != LGLSXP) {
        Rf_error("argument 'trace' should be either TRUE or FALSE.");
    }

    bool enable = LOGICAL(r_trace)[0];

    Trace* trace = new Trace(enable);

    SEXP r_res = R_NilValue;

    for (int i = 0; i < size; ++i) {
        clause_t clause = matcher->clauses[i];

        pattern_t pattern = clause->pattern;

        Value* value = new RawValue(r_value);

        Context* context = pattern_match(pattern, value, trace);

        // TODO: deletion should happen even if R code raises an error, so use
        // R's internal API.
        delete value;

        if (context->is_false()) {
            delete context;
            continue;
        }

        SEXP r_env = context->to_env(matcher->r_eval_env);
        delete context;

        r_res = Rf_eval(clause->r_expr, r_env);
        break;
    }

    delete trace;
    return r_res;
}

Context* pattern_match(pattern_t pattern, Value* value, Trace* trace) {
    trace->enter_pattern(pattern);
    Context* context = pattern_match_inner(pattern, value, trace);
    trace->exit_pattern(context);
    return context;
}

Context* pattern_match_inner(pattern_t pattern, Value* value, Trace* trace) {
    switch (pattern->type) {
        // id always matches and binds the value
    case pattern_type_t::ID: {
        Context* context = new Context(true);

        const std::string& name = pattern->id;

        if (name != ".") {
            context->bind(name, value);
        }

        return context;
    }

    case pattern_type_t::LGLVAL:
        return new Context(value->is_lgl1(pattern->lgl_val));

    case pattern_type_t::INTVAL:
        return new Context(value->is_int1(pattern->int_val));

    case pattern_type_t::DBLVAL:
        return new Context(value->is_dbl1(pattern->dbl_val));

    case pattern_type_t::STRVAL:
        return new Context(value->is_str1(pattern->str_val));

    case pattern_type_t::CPXVAL:
        return new Context(value->is_cpx1(pattern->cpx_val));

    case pattern_type_t::RAWVAL:
        return new Context(value->is_raw1(pattern->raw_val));

    case pattern_type_t::NA_POLY:
        return new Context(value->is_na1());

    case pattern_type_t::REIM: {
        if (value->get_type() != CPLXSXP || value->get_size() != 1) {
            return new Context(false);
        }

        Value* real_value = new ComplexValue(value, true);
        Context* real_ctxt =
            pattern_match(pattern->patterns[0], real_value, trace);
        delete real_value;

        if (real_ctxt->is_false()) {
            return real_ctxt;
        }

        Value* imag_value = new ComplexValue(value, false);
        Context* imag_ctxt =
            pattern_match(pattern->patterns[1], imag_value, trace);
        delete imag_value;

        if (imag_ctxt->is_false()) {
            delete real_ctxt;
            return imag_ctxt;
        }

        real_ctxt->consume(imag_ctxt);
        delete imag_ctxt;
        return real_ctxt;
    }

    case pattern_type_t::ANY: {
        for (int i = 0; i < pattern_size(pattern); ++i) {
            pattern_t subpat = pattern_at(pattern, i);
            Context* context = pattern_match(subpat, value, trace);
            if (context->is_true())
                return context;
            delete context;
        }
        return new Context(false);
    }

    case pattern_type_t::ALL: {
        Context* context = new Context(true);

        for (int i = 0; i < pattern_size(pattern) && context->is_true(); ++i) {
            pattern_t subpat = pattern_at(pattern, i);

            Context* temp = pattern_match(subpat, value, trace);

            if (temp->is_false()) {
                delete context;
                return temp;
            }

            context->consume(temp);
            delete temp;
        }

        return context;
    }

    case pattern_type_t::NONE: {
        bool state = false;

        for (int i = 0; i < pattern_size(pattern); ++i) {
            pattern_t subpat = pattern_at(pattern, i);

            Context* context = pattern_match(subpat, value, trace);

            bool elt_state = context->is_true();

            delete context;

            if (elt_state) {
                state = elt_state;
                break;
            }
        }
        // if none of the patterns match, the overall match is successful.
        return new Context(!state);
    }

    case pattern_type_t::LGLVEC:
        return match_vec(pattern, value, LGLSXP, trace);

    case pattern_type_t::INTVEC:
        return match_vec(pattern, value, INTSXP, trace);

    case pattern_type_t::DBLVEC:
        return match_vec(pattern, value, REALSXP, trace);

    case pattern_type_t::STRVEC:
        return match_vec(pattern, value, STRSXP, trace);

    case pattern_type_t::CPXVEC:
        return match_vec(pattern, value, CPLXSXP, trace);

    case pattern_type_t::RAWVEC:
        return match_vec(pattern, value, RAWSXP, trace);

    case pattern_type_t::RANGE:
        return match_range(pattern, value, trace);
    }
}

Context*
match_vec(pattern_t pattern, Value* value, SEXPTYPE type, Trace* trace) {
    /* is_slice check ensures that vec nested inside vec will never match.
     */
    if (value->get_type() != type || value->is_slice()) {
        return new Context(false);
    }

    Value* slice = value->take(value->get_size());
    Context* context = match_slice(pattern, 0, slice, trace);
    delete slice;
    return context;
}

Context*
match_slice(pattern_t pattern, int pat_idx, Value* value, Trace* trace) {
    int pat_size = pattern_size(pattern);

    // if all patterns have been matched but the vector still has unmatched
    // elements, then pattern matching is unsuccessful.
    if (pat_idx == pat_size) {
        return new Context(value->get_size() == 0);
    }

    pattern_t elt = pattern_at(pattern, pat_idx);
    range_t range = pattern_range(elt);

    /* if the number of vector elements is less than the minimum number of
       elements the pattern can match, then the match is unsuccessful */
    if (value->get_size() < range.min) {
        return new Context(false);
    }

    int low = range.min;
    int high = std::min(range.max, value->get_size());

    for (int i = high; i >= low; --i) {
        /* check if first i elements match the pattern */
        Value* left_slice = value->take(i);
        Context* head = pattern_match(elt, left_slice, trace);
        delete left_slice;

        /* check if rest of the elements match the remaining patterns */
        if (head->is_true()) {
            Value* right_slice = value->drop(i);
            Context* tail =
                match_slice(pattern, pat_idx + 1, right_slice, trace);
            delete right_slice;

            if (tail->is_true()) {
                head->consume(tail);
                delete tail;
                return head;
            }
            delete tail;
        }
        delete head;
    }

    return new Context(false);
}

Context* match_range(pattern_t pattern, Value* value, Trace* trace) {
    pattern_t sub_pat = pattern->patterns[0];

    std::vector<Context*> contexts;

    for (int i = 0; i < value->get_size(); ++i) {

        Value* elt = value->pick(i);
        Context* context = pattern_match(sub_pat, elt, trace);
        delete elt;

        contexts.push_back(context);

        if (context->is_false()) {
            break;
        }
    }

    Context* res = nullptr;

    if (contexts.size() > 0 && contexts.back()->is_false()) {
        res = new Context(false);
    } else {
        res = new Context(true);

        std::vector<std::string> ids = pattern_ids(sub_pat);

        for (const std::string& id: ids) {
            RangeValue* value = new RangeValue();

            for (Context* ctxt: contexts) {
                const Value* val = ctxt->lookup(id, nullptr);

                if (val == nullptr) {
                    Rf_error("identifier %s not bound in context", id.c_str());
                }

                value->push_back(val);
            }

            res->bind(id, value);
        }
    }

    for (int i = 0; i < contexts.size(); ++i) {
        Context* context = contexts[i];
        delete context;
    }

    return res;
}
