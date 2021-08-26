#ifndef MATCHR_MATCHR_H
#define MATCHR_MATCHR_H

#include "r.h"

SEXP matchr_initialize(SEXP r_package_environment);

SEXP matchr_finalize();

SEXP matchr_create(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env);

SEXP matchr_match(SEXP r_matcher, SEXP r_value);

#endif /* MATCHR_MATCHR_H */
