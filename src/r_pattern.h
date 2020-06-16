#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_pattern_create_pattern(SEXP r_expression);
SEXP r_pattern_get_expression(SEXP r_pattern);
}
