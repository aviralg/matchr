#ifndef MATCHR_UTILITIES_H
#define MATCHR_UTILITIES_H

#include "r.h"

void initialize_utilities(SEXP r_package_environment);

SEXP new_dot_env(bool hash, int size, SEXP r_parent);

#endif /* MATCHR_UTILITIES_H */
