#include "pattern.h"

struct pattern_impl_t {
    pattern_type_t type;
    union {
        struct {
        } null;

        struct {
            int value;
        } integer;

        struct {
            int value;
        } logical;

        struct {
            Rbyte value;
        } raw;

        struct {
            double value;
        } real;

        struct {
            SEXP r_value;
        } character;

        struct {
            Rcomplex value;
        } complex;

        struct {
            pattern_t formals;
            pattern_t body;
            pattern_t environment;
        } closure;

        struct {
            SEXPTYPE type;
            pattern_t sequence;
        } collection;

        struct {
            pattern_t pattern;
        } group;

        struct {
            std::string identifier;
        } wildcard;

        struct {
            pattern_t pattern;
            int min;
            int max;
        } range;

        struct {
            pattern_t real;
            pattern_t imaginary;
        } reim;

        struct {
            std::vector<pattern_t> patterns;
        } all;

        struct {
            std::vector<pattern_t> patterns;
        } any;

        struct {
            std::vector<pattern_t> patterns;
        } none;

        struct {
            SEXP r_predicate;
        } when;

        struct {
            pattern_t pattern;
            SEXP r_predicate;
        } then;

        struct {
            SEXP r_generator;
            pattern_t pattern;
        } with;

        // TODO
        // struct {
        //     pattern_t condition;
        //     pattern_t consequent;
        //     pattern_t alternative;
        // } if_else;
    } data;

    Identifiers identifier;
    Range range;
    SEXP r_expression;
};

namespace pattern {
pattern_t create(pattern_type_t type, SEXP r_expression) {
    pattern_t pattern = new pattern_impl_t();
    pattern->type = pattern_type_t::Null;
    pattern->range = Range(1, 1);
    pattern->r_expression = r_expression;
}

pattern_t from_null(SEXP r_expression) {
    pattern_t pattern = create(pattern_type_t::Null, r_expression);
    return pattern;
}

pattern_t from_integer(SEXP r_expression, int value) {
    pattern_t pattern = create(pattern_type_t::Integer, r_expression);
    pattern->data.integer.value = value;
    return pattern;
}

pattern_t from_logical(SEXP r_expression, int value) {
    pattern_t pattern = create(pattern_type_t::Logical, r_expression);
    pattern->data.logical.value = value;
    return pattern;
}

pattern_t from_raw(SEXP r_expression, Rbyte value) {
    pattern_t pattern = create(pattern_type_t::Raw, r_expression);
    pattern->data.raw.value = value;
    return pattern;
}

pattern_t from_real(SEXP r_expression, double value) {
    pattern_t pattern = create(pattern_type_t::Real, r_expression);
    pattern->data.real.value = value;
    return pattern;
}

pattern_t from_character(SEXP r_expression, SEXP r_value) {
    pattern_t pattern = create(pattern_type_t::Character, r_expression);
    pattern->data.character.r_value = r_value;
    return pattern;
}

pattern_t from_complex(SEXP r_expression, const Rcomplex& value) {
    pattern_t pattern = create(pattern_type_t::Complex, r_expression);
    pattern->data.complex.value = value;
    return pattern;
}

pattern_t from_closure(SEXP r_expression,
                       pattern_t formals,
                       pattern_t body,
                       pattern_t environment) {
    pattern_t pattern = create(pattern_type_t::Closure, r_expression);
    pattern->data.closure.formals = formals;
    pattern->data.closure.body = body;
    pattern->data.closure.environment = environment;
    pattern->identifiers.merge(get_identifiers(formals))
        .merge(get_identifiers(body))
        .merge(get_identifiers(environment));
    return pattern;
}

pattern_t
from_collection(SEXP r_expression, SEXPTYPE type, pattern_t sequence) {
    pattern_t pattern = create(pattern_type_t::Collection, r_expression);
    pattern->data.collection.type = type;
    pattern->data.collection.sequence = sequence;
    pattern->identifiers = get_identifiers(sequence);
    return pattern;
}

pattern_t from_group(SEXP r_expression, pattern_t pattern) {
    pattern_t pattern = create(pattern_type_t::Group, r_expression);
    pattern->data.group.pattern = pattern;
    pattern->identifiers = get_identifiers(pattern);
    return pattern;
}

pattern_t from_wildcard(SEXP r_expression, const std::string& identifier) {
    pattern_t pattern = create(pattern_type_t::Wildcard, r_expression);
    pattern->data.wildcard.identifier = identifier;
    if (identifier != ".") {
        pattern->identifiers.add(identifier);
    }
    return pattern;
}

pattern_t
from_range(SEXP r_expression, pattern_t pattern, const int min, const int max) {
    pattern_t pat = create(pattern_type_t::Range, r_expression);
    pat->data.range.pattern = pattern;
    pat->data.range.min = min;
    pat->data.range.max = max;
    pat->identifiers = get_identifiers(pattern);

    return pat;
}

pattern_t from_reim(SEXP r_expression, pattern_t real, pattern_t imaginary) {
    pattern_t pattern = create(pattern_type_t::Reim, r_expression);
    pattern->data.reim.real = real;
    pattern->data.reim.imaginary = imaginary;
    pattern->identifiers.merge(get_identifiers(real))
        .merge(get_identifiers(imaginary));
    return pattern;
}

pattern_t from_all(SEXP r_expression, const std::vector<pattern_t>& patterns) {
    pattern_t pattern = create(pattern_type_t::All, r_expression);
    pattern->data.all.patterns = patterns;
    for (int i = 0; i < patterns.size(); ++i) {
        pattern->identifiers.merge(get_identifiers(patterns[i]));
    }
    return pattern;
}

pattern_t from_any(SEXP r_expression, const std::vector<pattern_t>& patterns) {
    pattern_t pattern = create(pattern_type_t::Any, r_expression);
    pattern->data.any.patterns = patterns;
    if (patterns.size() > 0) {
        pattern->identifiers = get_identifiers(patterns[0]);
    }
    return pattern;
}

pattern_t from_none(SEXP r_expression, const std::vector<pattern_t>& patterns) {
    pattern_t pattern = create(pattern_type_t::None, r_expression);
    pattern->data.none.patterns = patterns;
    for (int i = 0; i < patterns.size(); ++i) {
        pattern->identifiers.merge(get_identifiers(patterns[i]));
    }
    return pattern;
}

pattern_t from_when(SEXP r_expression, SEXP r_predicate) {
    pattern_t pattern = create(pattern_type_t::When, r_expression);
    pattern->data.when.r_predicate = r_predicate;
    return pattern;
}

pattern_t from_then(SEXP r_expression, pattern_t pattern, SEXP r_predicate) {
    pattern_t pattern = create(pattern_type_t::Then, r_expression);
    pattern->data.then.pattern = pattern;
    pattern->data.then.r_predicate = r_predicate;
    pattern->identifiers = get_identifiers(pattern);
    return pattern;
}

pattern_t from_with(SEXP r_expression, SEXP r_generator, pattern_t pattern) {
    pattern_t pattern = create(pattern_type_t::With, r_expression);
    pattern->data.with.r_generator = r_generator;
    pattern->data.with.pattern = pattern;
    pattern->identifiers = get_identifiers(pattern);
    return pattern;
}

const Identifiers& get_identifiers(pattern_t pattern) {
    return pattern->identifiers;
}

const Range& get_range(pattern_t pattern) {
    return pattern->range;
}

SEXP get_expression(pattern_t pattern) {
    return pattern->r_expression;
}

SEXP bind_input(input_t input, SEXP r_pat_env) {
    SEXP r_value = PROTECT(input::to_sexp(input));

    SEXP r_env = PROTECT(new_environment(true, 1, r_pat_env));

    Rf_setVar(DotSymbol, r_value, r_env);

    UNPROTECT(2);

    return r_env;
}

bool is_vector_type(const SEXPTYPE type) {
    return type == INTSXP || type == REALSXP || type == LGLSXP ||
           type == RAWSXP || type == STRSXP || type == CPLXSXP;
}

bool is_logical_true(SEXP r_value) {
    return (TYPEOF(r_result) == LGLSXP) && (Rf_length(r_result) == 1) &&
           (LOGICAL(r_result)[0] == 1);
}

Context match_null(input_t input, SEXP r_pat_env) {
    return Context(input::get_type(input) == NILSXP);
}

Context match_integer(const int value, input_t input, SEXP r_pat_env) {
    return Context(input::is_scalar_integer(input, value));
}

Context match_logical(const int value, input_t input, SEXP r_pat_env) {
    return Context(input::is_scalar_logical(input, value));
}

Context match_raw(const Rbyte value, input_t input, SEXP r_pat_env) {
    return Context(input::is_scalar_raw(input, value));
}

Context match_real(const double value, input_t input, SEXP r_pat_env) {
    return Context(input::is_scalar_real(input, value));
}

Context match_character(SEXP r_value, input_t input, SEXP r_pat_env) {
    return Context(input::is_scalar_character(input, r_value));
}

Context match_complex(const Rcomplex& value, input_t input, SEXP r_pat_env) {
    return Context(input::is_scalar_complex(input, value));
}

Context match_closure(pattern_t formals,
                      pattern_t body,
                      pattern_t environment,
                      input_t input,
                      SEXP r_pat_env) {
    if (input::get_type(input) != CLOSXP) {
        return Context(false);
    }

    if (formals != nullptr) {
        Context context =
            match(formals, input::get_closure_formals(input), r_pat_env);
    }

    if (context && body != nullptr) {
        context = context.merge(
            match(body, input::get_closure_body(input), r_pat_env));
    }

    if (context && environment != nullptr) {
        context = context.merge(match(
            environment, input::get_closure_environment(input), r_pat_env));
    }

    return context;
}

Context match_collection(const SEXPTYPE type,
                         pattern_t sequence,
                         input_t input,
                         SEXP r_pat_env) {
    SEXPTYPE input_type = input::get_type(input);

    if (type == ANYSXP) {
        if (is_vector_type(input_type)) {
            return match_sequence(sequence, input, r_pat_env);
        }

        else {
            return Context(false);
        }
    }

    else if (type == input_type) {
        return match_sequence(sequence, input, r_pat_env);
    }

    else {
        return Context(false);
    }
}

Context match_group(pattern_t pattern, input_t input, SEXP r_pat_env) {
    return match(pattern, input, r_pat_env);
}

Context
match_wildcard(const std::string& identifier, input_t input, SEXP r_pat_env) {
    Context context(true);

    if (name != ".") {
        context.get_bindings().bind(identifier, input::to_output(input));
    }

    return context;
}

Context
match_reim(pattern_t real, pattern_t imaginary, input_t input, SEXP r_pat_env) {
    if (input::get_type(input) != CPLXSXP) {
        return Context(false);
    }

    input_t sub_input = input::from_complex(input, 1);

    Context real_ctxt = match(real, sub_input, r_pat_env);

    if (!real_ctxt) {
        return real_ctxt;
    }

    input::destroy(sub_input);

    sub_input = input::from_complex(input, 0);

    Context imag_ctxt = match(imaginary, sub_input, r_pat_env);

    input::destroy(sub_input);

    if (!imag_ctxt) {
        return imag_ctxt;
    }

    return real_ctxt.merge(imag_ctxt);
}

Context match_all(const std::vector<pattern_t>& patterns,
                  input_t input,
                  SEXP r_pat_env) {
    Context context(true);

    for (int i = 0; context && i < patterns.size(); ++i) {
        pattern_t pattern = patterns[i];
        context = context.merge(match(pattern, input, r_pat_env));
    }

    return context;
}

Context match_any(const std::vector<pattern_t>& patterns,
                  input_t input,
                  SEXP r_pat_env) {
    Context context(false);

    for (int i = 0; i < patterns.size() && !context; ++i) {
        pattern_t pattern = patterns[i];
        context = match(pattern, input, r_pat_env);
    }

    return context;
}

Context match_none(const std::vector<pattern_t>& patterns,
                   input_t input,
                   SEXP r_pat_env) {
    Context context(false);

    for (int i = 0; i < patterns.size() && !context; ++i) {
        pattern_t pattern = patterns[i];
        context = match(pattern, input, r_pat_env);
    }

    context.toggle_status();

    return context;
}

Context match_when(SEXP r_predicate, input_t input, SEXP r_pat_env) {
    /* TODO: add env with . */

    SEXP r_env = PROTECT(bind_input(input, r_pat_env));

    SEXP r_result = PROTECT(safe_eval(r_predicate, r_env, input, nullptr, 1));

    bool status = is_logical_true(r_result);

    UNPROTECT(2);

    return Context(status);
}

// Context match_if_else(pattern_t condition,
//                       pattern_t consequent,
//                       pattern_t alternative,
//                       input_t input,
//                       SEXP r_pat_env) {
//     Context context_if = match(condition, input, r_pat_env);
//
//     if (context_if) {
//         return match(consequent, input, r_pat_env);
//     }
//
//     else if (alternative != nullptr) {
//         return match(alternative, input, r_pat_env);
//     }
//
//     else {
//         return Context(false);
//     }
// }

Context
match_then(pattern_t pattern, SEXP r_predicate, input_t input, SEXP r_pat_env) {
    Context context = match(pattern, input, r_pat_env);

    if (!context) {
        return context;
    }

    SEXP r_eval_env = PROTECT(context.get_bindings().as_environment(r_pat_env));

    r_eval_env = PROTECT(bind_input(input, r_eval_env));

    SEXP r_result =
        PROTECT(safe_eval(r_predicate, r_eval_env, input, nullptr, 2));

    bool status = is_logical_true(r_result);

    UNPROTECT(3);

    if (status) {
        return context;
    }

    return Context(false);
}

Context match_with(SEXP r_generator,
                   pattern_t pattern,
                   input_t input,
                   SEXP r_pat_env) const override final {
    /* TODO: add env with . */

    SEXP r_eval_env = PROTECT(bind_dot(input, r_pat_env));

    SEXP r_result =
        PROTECT(safe_eval(r_generator, r_eval_env, input, nullptr, 1));

    input_t new_input = input::from_sexp(r_result);

    /* TODO: cleanup of input and protect will not happen if error happens
     * in this match. */
    Context result = match(pattern, new_input, r_pat_env);

    UNPROTECT(2);

    input::destroy(new_input);

    return result;
}

Context match(pattern_t pattern, input_t input, SEXP r_pat_env) {
    switch (pattern->type) {
    case pattern_type_t::Null:
        return match_null(input, r_pat_env);
        break;

    case pattern_type_t::Integer:
        return match_integer(pattern->data.integer.value, input, r_pat_env);
        break;

    case pattern_type_t::Logical:
        return match_logical(pattern->data.logical.value, input, r_pat_env);
        break;

    case pattern_type_t::Raw:
        return match_raw(pattern->data.raw.value, input, r_pat_env);
        break;

    case pattern_type_t::Real:
        return match_real(pattern->data.real.value, input, r_pat_env);
        break;

    case pattern_type_t::Character:
        return match_character(
            pattern->data.character.r_value, input, r_pat_env);
        break;

    case pattern_type_t::Complex:
        return match_real(pattern->data.complex.value, input, r_pat_env);
        break;

    case pattern_type_t::Closure:
        return match_closure(pattern->data.closure.formals,
                             pattern->data.closure.body,
                             pattern->data.closure.environment,
                             input,
                             r_pat_env);
        break;

    case pattern_type_t::Collection:
        return match_collection(pattern->data.collection.type,
                                pattern->data.collection.sequence,
                                input,
                                r_pat_env);
        break;

    case pattern_type_t::Group:
        return match_group(pattern->data.group.pattern, input, r_pat_env);
        break;

    case pattern_type_t::Wildcard:
        return match_group(pattern->data.wildcard.name, input, r_pat_env);
        break;

    case pattern_type_t::Reim:
        return match_reim(pattern->data.reim.real,
                          pattern->data.reim.imaginary,
                          input,
                          r_pat_env);
        break;

    case pattern_type_t::All:
        return match_and(pattern->data.all.patterns, input, r_pat_env);
        break;

    case pattern_type_t::Any:
        return match_any(pattern->data.any.patterns, input, r_pat_env);
        break;

    case pattern_type_t::None:
        return match_none(pattern->data.none.patterns, input, r_pat_env);
        break;

    case pattern_type_t::When:
        return match_when(pattern->data.when.r_predicate, input, r_pat_env);
        break;

    case pattern_type_t::Then:
        return match_then(pattern->data.then.pattern,
                          pattern->data.then.r_predicate,
                          input,
                          r_pat_env);
        break;

    case pattern_type_t::With:
        return match_with(pattern->data.with.r_generator,
                          pattern->data.with.pattern,
                          input,
                          r_pat_env);
        break;
    }
}

result_t parse_helper(SEXP r_expression);

result_t parse_closure_pattern(const std::string& function_name,
                               SEXP r_expression) {
    result_t first = parse_helper(CADR(r_expression));

    if (result::has_error(first)) {
        return first;
    }

    result_t second = parse_helper(CADDR(r_expression));

    if (result::has_error(second)) {
        pattern::destroy(result::get_pattern(first));
        return second;
    }

    result_t third = parse_helper(CADDDR(r_expression));

    if (result::has_error(third)) {
        pattern::destroy(result::get_pattern(first));
        pattern::destroy(result::get_pattern(second));
        return third;
    }

    return result::from_pattern(from_closure(r_expression,
                                             result::get_pattern(first),
                                             result::get_pattern(second),
                                             result::get_pattern(third)));
}

result_t parse_group_pattern(const std::string& function_name,
                             SEXP r_expression) {
    result_t inner = parse_helper(CADR(r_expression));

    if (result::has_message(inner)) {
        return inner;
    }

    return result::from_pattern(
        from_group(r_expression, result::get_pattern(inner)));
}

result_t parse_when_pattern(const std::string& function_name,
                            SEXP r_expression) {
    if (Rf_length(r_expression) != 3) {
        return result::from_pattern(
            "incorrect number of arguments passed to when");
    }

    result_t inner = parse_helper(CADDR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return result::from_pattern(new WhenUnaryPattern(
        r_expression, CADR(r_expression), inner.get_pattern()));
}

result_t parse_then_pattern(const std::string& function_name,
                            SEXP r_expression) {
    if (Rf_length(r_expression) != 3) {
        return result::from_pattern(
            "incorrect number of arguments passed to then");
    }

    result_t inner = parse_helper(CADR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return result::from_pattern(new ThenUnaryPattern(
        r_expression, inner.get_pattern(), CADDR(r_expression)));
}

result_t parse_with_pattern(const std::string& function_name,
                            SEXP r_expression) {
    if (Rf_length(r_expression) != 3) {
        return result::from_pattern(
            "incorrect number of arguments passed to with");
    }

    result_t inner = parse_helper(CADDR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return result::from_pattern(new WithUnaryPattern(
        r_expression, CADR(r_expression), inner.get_pattern()));
}

result_t
parse_variadic_pattern(pattern_t (*create)(SEXP, const std::vector<pattern_t>&),
                       const std::string& function_name,
                       SEXP r_expression) {
    std::vector<pattern_t> patterns;

    for (SEXP r_tail = CDR(r_expression); r_tail != R_NilValue;
         r_tail = CDR(r_tail)) {
        SEXP r_head = CAR(r_tail);

        result_t result = parse_helper(r_head);

        if (result::has_message(result)) {
            for (int i = 0; i < patterns.size(); ++i) {
                destroy(patterns[i]);
            }
            patterns.clear();
            return result;
        }

        patterns.push_back(result::get_pattern(result));
    }

    return result::from_pattern(create(r_expression, patterns));
}

std::tuple<int, int> parse_range_limits(SEXP r_expression) {
    std::string identifier = CHAR(PRINTNAME(r_expression));

    if (identifier[0] != '.' || identifier[1] != '.' || identifier[2] != '.') {
        return {-1, -1};
    }

    int length = identifier.size();
    int i = 3;
    int min = 0;

    if (i == length) {
        return {0, 0};
    }

    for (; i < length && isdigit(identifier[i]); ++i) {
        min = min * 10 + (identifier[i] - '0');
    }

    if (i < length && identifier[i] != '_') {
        return {-1, -1};
    }

    if (i == length) {
        return {min, INT_MAX};
    }

    /* for _ */
    ++i;

    /* if the pattern is ..<min>_ */
    if (i == length) {
        return {-1, -1};
    }

    int max = 0;
    for (; i < length && isdigit(identifier[i]); ++i) {
        max = max * 10 + (identifier[i] - '0');
    }

    if (i != length) {
        return {-1, -1};
    }

    return {min, max};
}

template <typename T>
result_t parse_greedy_pattern_sequence(const std::string& function_name,
                                       SEXP r_expression) {
    std::vector<Pattern*> patterns;

    bool range = false;
    for (SEXP r_tail = CDR(r_expression); r_tail != R_NilValue;
         r_tail = CDR(r_tail)) {
        SEXP r_head = CAR(r_tail);

        if (TYPEOF(r_head) == SYMSXP) {
            int min;
            int max;

            std::tie(min, max) = parse_range_limits(r_head);

            if (min != -1 && max != -1) {
                if (!range) {
                    for (int i = 0; i < patterns.size(); ++i) {
                        delete patterns[i];
                    }
                    return result::from_pattern("expected pattern, not ...");
                }

                if (min < 0 || max < 0 || max < min) {
                    for (int i = 0; i < patterns.size(); ++i) {
                        delete patterns[i];
                    }
                    return result::from_pattern(
                        "invalid minimum or maximum for range");
                }

                pattern_t pattern =
                    from_range(r_head, patterns.back(), min, max);
                patterns.back() = pattern;
                range = false;
                continue;
            }
        }

        result_t result = parse_helper(r_head);

        if (result.has_error()) {
            for (int i = 0; i < patterns.size(); ++i) {
                delete patterns[i];
            }
            patterns.clear();
            return result;
        }

        patterns.push_back(result.get_pattern());
        range = true;
    }

    return result::from_pattern(
        new T(r_expression, new SequencePattern(CDR(r_expression), patterns)));
}

template <typename T>
result_t parse_sequence_pattern(const std::string& function_name,
                                SEXP r_expression) {
    result_t result =
        parse_variadic_pattern<SequencePattern>(function_name, r_expression);

    if (result.has_error()) {
        return result;
    }

    return result::from_pattern(new T(r_expression, result.get_pattern()));
}

result_t parse_wildcard_pattern(SEXP r_expression) {
    std::string identifier(CHAR(PRINTNAME(r_expression)));
    return result::from_pattern(new WildcardPattern(r_expression, identifier));
}

result_t parse_helper(SEXP r_expression) {
    SEXPTYPE expr_type = TYPEOF(r_expression);

    if (expr_type == NILSXP) {
        return result::from_pattern(from_null(r_expression));
    }

    else if (expr_type == SYMSXP) {
        return parse_wildcard_pattern(r_expression);
    }

    else if (expr_type == LGLSXP && Rf_length(r_expression) == 1) {
        return result::from_pattern(
            from_logical(r_expression, asLogical(r_expression)));
    }

    else if (expr_type == INTSXP && Rf_length(r_expression) == 1) {
        return result::from_pattern(
            from_integer(r_expression, asInteger(r_expression)));
    }

    else if (expr_type == REALSXP && Rf_length(r_expression) == 1) {
        return result::from_pattern(
            from_real(r_expression, asReal(r_expression)));
    }

    else if (expr_type == STRSXP && Rf_length(r_expression) == 1) {
        return result::from_pattern(
            from_character(r_expression, STRING_ELT(r_expression, 0)));
    }

    else if (expr_type == LANGSXP) {
        /* TODO: check if the CAR is a symbol or not */
        std::string function_name = CHAR(PRINTNAME(CAR(r_expression)));

        if (function_name == "closure" || function_name == "clo") {
            return parse_closure_pattern(function_name, r_expression);
        }

        else if (function_name == "(") {
            return parse_group_pattern(function_name, r_expression);
        }

        else if (function_name == "when") {
            return parse_when_pattern(function_name, r_expression);
        }

        else if (function_name == "then") {
            return parse_then_pattern(function_name, r_expression);
        }

        else if (function_name == "with") {
            return parse_with_pattern(function_name, r_expression);
        }

        else if (function_name == "&&" || function_name == "all") {
            return parse_variadic_pattern(
                from_all, function_name, r_expression);
        }

        else if (function_name == "||" || function_name == "any") {
            return parse_variadic_pattern(
                from_any, function_name, r_expression);
        }

        else if (function_name == "!" || function_name == "none") {
            return parse_variadic_pattern(
                from_none, function_name, r_expression);
        }

        else if (function_name == "vector" || function_name == "vec") {
            return parse_sequence_pattern(ANYSXP, function_name, r_expression);
        }

        else if (function_name == "integer" || function_name == "int") {
            return parse_sequence_pattern(INTSXP, function_name, r_expression);
        }

        else if (function_name == "double" || function_name == "dbl" ||
                 function_name == "real") {
            return parse_sequence_pattern(REALSXP, function_name, r_expression);
        }

        else if (function_name == "logical" || function_name == "lgl") {
            return parse_sequence_pattern(LGLSXP, function_name, r_expression);
        }

        else if (function_name == "raw") {
            return parse_sequence_pattern(RAWSXP, function_name, r_expression);
        }

        else if (function_name == "character" || function_name == "chr") {
            return parse_sequence_pattern(STRSXP, function_name, r_expression);
        }

        else if (function_name == "list") {
            return parse_sequence_pattern(VECSXP, function_name, r_expression);
        }

        else if (function_name == "pairlist" || function_name == "pair") {
            return parse_sequence_pattern(LISTSXP, function_name, r_expression);
        }

        else if (function_name == "language" || function_name == "lang") {
            return parse_sequence_pattern(LANGSXP, function_name, r_expression);
        }
    }

    std::string message = "invalid pattern";
    return result::from_message(message);
}

pattern_t parse(SEXP r_expression) {
    result_t result = parse_helper(r_expression);
    if (result::has_message(result)) {
        Rf_error("%s", result::get_message(result).c_str());
    }

    return result::get_pattern(result);
}

} // namespace pattern
