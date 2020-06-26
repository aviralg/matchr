#include "r.h"

extern "C" {
SEXP r_matcher_create_matcher(SEXP r_clauses);
SEXP r_matcher_get_clauses(SEXP r_matcher);
SEXP r_matcher_match_value(SEXP r_matcher, SEXP r_value);
}
