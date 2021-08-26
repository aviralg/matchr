#include "utilities.h"
#include "Matcher.h"

bool initialized = false;

extern "C" {

#include "matchr.h"

SEXP matchr_initialize(SEXP r_package_environment) {
    if (!initialized) {
        initialized = true;
        initialize_utilities(r_package_environment);
    }

    return R_NilValue;
}

SEXP matchr_finalize() {
    if (initialized) {
        initialized = false;
    }

    return R_NilValue;
}

SEXP matchr_create(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env) {
    Matcher* matcher = Matcher::create(r_clauses, r_pat_env, r_eval_env);
    return Matcher::to_sexp(matcher);
}

SEXP matchr_match(SEXP r_matcher, SEXP r_value) {
    Matcher* matcher = Matcher::from_sexp(r_matcher);
    return matcher->match(r_value);
}
}
