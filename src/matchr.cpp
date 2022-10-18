#include "utilities.h"
#include "matcher.h"

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
    matcher_t matcher = matcher::parse(r_clauses, r_pat_env, r_eval_env);
    return matcher::to_sexp(matcher);
}

SEXP matchr_match(SEXP r_matcher, SEXP r_value) {
    matcher_t matcher = matcher::from_sexp(r_matcher);
    return matcher::match(matcher, r_value);
}
}
