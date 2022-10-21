#include "matchr.h"
#include "matcher.h"
#include "Value.h"
#include "Context.h"

Context* pattern_match(pattern_t pattern, Value* value);
Context* match_slice(pattern_t pattern, int pat_idx, Value* value);

SEXP r_matchr_match(SEXP r_matcher, SEXP r_value) {
    matcher_t matcher = matcher_unwrap(r_matcher);

    int size = matcher->clauses.size();

    for (int i = 0; i < size; ++i) {
        clause_t clause = matcher->clauses[i];

        pattern_t pattern = clause->pattern;

        Value* value = new Value(r_value);

        Context* context = pattern_match(pattern, value);

        // TODO: deletion should happen even if R code raises an error, so use
        // R's internal API.
        delete value;

        if (context->is_false()) {
            delete context;
            continue;
        }

        SEXP r_env = context -> to_env(matcher->r_eval_env);
        delete context;

        return Rf_eval(clause->r_expr, r_env);
    }

    return R_NilValue;
}

Context* pattern_match(pattern_t pattern, Value* value) {
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

    case pattern_type_t::ANY: {
        for (int i = 0; i < pattern_size(pattern); ++i) {
            pattern_t subpat = pattern_at(pattern, i);
            Context* context = pattern_match(subpat, value);
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

            Context* temp = pattern_match(subpat, value);

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

            Context* context = pattern_match(subpat, value);

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

    case pattern_type_t::LGLVEC: {
        /* is_slice check ensures that vec nested inside vec will never match.
         */
        if (value->get_type() != LGLSXP || value->is_slice()) {
            return new Context(false);
        }

        Value* slice = value->take(value->get_size());
        Context* context = match_slice(pattern, 0, slice);
        delete slice;
        return context;
    }

    case pattern_type_t::INTVEC: {
        /* is_slice check ensures that vec nested inside vec will never match.
         */
        if (value->get_type() != INTSXP || value->is_slice()) {
            return new Context(false);
        }

        Value* slice = value->take(value->get_size());
        Context* context = match_slice(pattern, 0, slice);
        delete slice;
        return context;
    }

    case pattern_type_t::DBLVEC: {
        /* is_slice check ensures that vec nested inside vec will never match.
         */
        if (value->get_type() != REALSXP || value->is_slice()) {
            return new Context(false);
        }

        Value* slice = value->take(value->get_size());
        Context* context = match_slice(pattern, 0, slice);
        delete slice;
        return context;
    }

    case pattern_type_t::RANGE: {
        Rf_error("range should be handled in pattern_seq_expr");
        return nullptr;
    }
    }
}

Context* match_slice(pattern_t pattern, int pat_idx, Value* value) {
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

    for (int i = low; i <= high; ++i) {
        /* check if first i elements match the pattern */
        Value* left_slice = value->take(i);
        Context* head = pattern_match(elt, left_slice);
        delete left_slice;

        /* check if rest of the elements match the remaining patterns */
        if (head->is_true()) {
            Value* right_slice = value->drop(i);
            Context* tail = match_slice(pattern, pat_idx + 1, right_slice);
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
