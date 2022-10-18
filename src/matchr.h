#ifndef MATCHR_MATCHR_H
#define MATCHR_MATCHR_H

#include "Rincludes.h"

#ifdef __cplusplus
extern "C" {
#endif

SEXP r_matchr_parse(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env);
SEXP r_matchr_match(SEXP r_matcher, SEXP r_value);
SEXP r_matchr_expr(SEXP r_matcher);

#ifdef __cplusplus
}
#endif

#endif /* MATCHR_MATCHR_H */
