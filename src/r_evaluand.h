#ifndef MATCHR_R_EVALUAND_H
#define MATCHR_R_EVALUAND_H

#include "r.h"

extern "C" {
SEXP r_evaluand_create_evaluand(SEXP r_expression, SEXP r_environment);
SEXP r_evaluand_get_expression(SEXP r_evaluand);
SEXP r_evaluand_get_environment(SEXP r_evaluand);
}

#endif /* MATCHR_R_EVALUAND_H  */
