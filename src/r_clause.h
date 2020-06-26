
#include "r.h"

extern "C" {
SEXP r_clause_create_clause(SEXP r_pattern, SEXP r_evaluand);
SEXP r_clause_get_pattern(SEXP r_clause);
SEXP r_clause_get_evaluand(SEXP r_clause);
}
