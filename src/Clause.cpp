#include "clause.h"

struct clause_impl_t {
    pattern_t pattern;
    SEXP r_expression;
};

namespace clause {

clause_t create(pattern_t pattern, SEXP r_expression) {
    clause_t clause = new clause_impl_t();
    clause->pattern = pattern;
    clause->r_expression = r_expression;
    return clause;
}

clause_t parse(SEXP r_expression) {
    if (TYPEOF(r_expression) != LANGSXP) {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    if (Rf_length(r_expression) != 3) {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    SEXP r_head = CAR(r_expression);

    if (TYPEOF(r_head) != SYMSXP) {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    std::string head = CHAR(PRINTNAME(r_head));

    if (head != "~") {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    pattern_t pattern = pattern::parse(CADR(r_expression));

    return create(pattern, CADDR(r_expression));
}

void destroy(clause_t clause) {
    pattern::destroy(clause->pattern);
    delete clause;
}

SEXP match(clause_t clause, SEXP r_value, SEXP r_pat_env, SEXP r_eval_env) {
    input_t input = input::from_sexp(r_value);

    Context context = pattern::match(clause->pattern, input, r_pat_env);

    input::destroy(input);

    if (context) {
        r_eval_env = PROTECT(context.get_bindings().as_environment(r_eval_env));

        SEXP r_result = PROTECT(
            safe_eval(clause->r_expression, r_eval_env, nullptr, nullptr, 1));

        UNPROTECT(2);

        return r_result;

    } else {
        return NULL;
    }
}

} // namespace clause
