#include "matcher.h"

struct matcher_impl_t {
    std::vector<clause_t> clauses;
    SEXP r_pat_env;
    SEXP r_eval_env;
};

namespace matcher {
matcher_t
create(const std::vector<clause_t>& clauses, SEXP r_pat_env, SEXP r_eval_env) {
    matcher_t matcher = new matcher_impl_t(clauses, r_pat_env, r_eval_env);
    R_PreserveObject(r_pat_env);
    R_PreserveObject(r_eval_env);
    return matcher;
}

void destroy(matcher_t matcher) {
    const std::vector<clause_t>& clauses = matcher->clauses;

    for (int index = 0; index < clauses.size(); ++index) {
        clause::destroy(clauses[index]);
    }

    R_ReleaseObject(matcher->r_pat_env);
    R_ReleaseObject(matcher->r_eval_env);

    delete matcher;
}

matcher_t parse(SEXP r_expression, SEXP r_pat_env, SEXP r_eval_env) {
    int count = LENGTH(r_clauses);
    std::vector<clause_t> clauses;
    clauses.reserve(count);

    for (int index = 0; index < count; ++index) {
        /* TODO: memory leak happens if create errors */
        clauses.push_back(clause::create(VECTOR_ELT(r_expression, index)));
    }

    return create(clauses, r_pat_env, r_eval_env);
}

SEXP match(matcher_t matcher, SEXP r_value) {
    const std::vector<clause_t>& clauses = matcher->clauses;
    SEXP r_pat_env = matcher->r_pat_env;
    SEXP r_eval_env = matcher->r_eval_env;

    for (int index = 0; index < clauses.size(); ++index) {
        clause_t clause = clauses[index];
        SEXP r_result = clause::match(clause, r_value, r_pat_env, r_eval_env);

        if (r_result != NULL) {
            return r_result;
        }
    }

    /* this should not happen since the last pattern is added to raise this
     * error explicitly */
    Rf_error("value did not match any pattern");
    return R_NilValue;
}

matcher_t from_sexp(SEXP r_matcher) {
    void* matcher = R_ExternalPtrAddr(r_matcher);
    if (matcher == NULL) {
        Rf_errorcall(R_NilValue, "null matcher object encountered");
    } else {
        return static_cast<matcher_t>(matcher);
    }
}

SEXP get_class() {
    SEXP classnames = PROTECT(allocVector(STRSXP, 1));
    SET_STRING_ELT(classnames, 0, mkChar("matchr_matcher"));
    UNPROTECT(1);
    return classnames;
}

void destroy_sexp(SEXP r_matcher) {
    void* matcher = R_ExternalPtrAddr(r_matcher);
    if (matcher != nullptr) {
        matcher::destroy(static_cast<matcher_t>(matcher));
        R_SetExternalPtrAddr(r_matcher, nullptr);
    }
}

SEXP to_sexp(matcher_t matcher) {
    SEXP r_matcher =
        PROTECT(R_MakeExternalPtr(matcher, R_NilValue, R_NilValue));
    R_RegisterCFinalizerEx(r_matcher, destroy_sexp, TRUE);
    setAttrib(r_matcher, R_ClassSymbol, get_class());
    UNPROTECT(1);

    return r_matcher;
}

} // namespace matcher
