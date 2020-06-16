#ifndef MATCHR_UTILITIES_H
#define MATCHR_UTILITIES_H

#include <Rinternals.h>

void initialize_utilities(SEXP r_package_environment);

SEXP new_dot_env(SEXP hash, SEXP parent, SEXP size);

#endif /* MATCHR_UTILITIES_H */
