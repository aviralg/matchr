#include "Matcher.h"

Matcher* Matcher::create(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env) {
    Matcher* matcher = new Matcher(r_pat_env, r_eval_env);
    int count = LENGTH(r_clauses);
    for (int index = 0; index < count; ++index) {
        Clause* clause = Clause::create(VECTOR_ELT(r_clauses, index));
        matcher->add_clause_(clause);
    }
    return matcher;
}

Matcher* Matcher::from_sexp(SEXP r_matcher) {
    void* matcher = R_ExternalPtrAddr(r_matcher);
    if (matcher == NULL) {
        Rf_errorcall(R_NilValue, "null matcher object encountered");
    } else {
        return static_cast<Matcher*>(matcher);
    }
}

SEXP get_class() {
    SEXP classnames = PROTECT(allocVector(STRSXP, 1));
    SET_STRING_ELT(classnames, 0, mkChar("matchr_matcher"));
    UNPROTECT(1);
    return classnames;
}

void destroy_sexp(SEXP r_matcher) {
    delete static_cast<Matcher*>(R_ExternalPtrAddr(r_matcher));
    R_SetExternalPtrAddr(r_matcher, NULL);
}

SEXP Matcher::to_sexp(Matcher* matcher) {
    SEXP r_matcher =
        PROTECT(R_MakeExternalPtr(matcher, R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_matcher, destroy_sexp, TRUE);

    setAttrib(r_matcher, R_ClassSymbol, get_class());

    UNPROTECT(1);

    return r_matcher;
}
