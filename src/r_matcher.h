#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_matcher_create_matcher(SEXP r_clauses);
SEXP r_matcher_get_clauses(SEXP r_matcher);
SEXP r_matcher_match_expression(SEXP r_matcher, SEXP r_expression);
}
