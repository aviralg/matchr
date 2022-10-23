#ifndef MATCHR_UTILS_H
#define MATCHR_UTILS_H

#include "Rincludes.h"

SEXP deparse(SEXP r_val, bool quote = true);

const char* str_unwrap(SEXP char_val);

SEXP str_wrap(const char* str_val);

char* str_dup(const char* val);

bool cmp_dbl(double val1, double val2);

bool cmp_cpx(const Rcomplex& val1, const Rcomplex& val2);

bool cmp_str(const char* val1, const char* val2);

#endif /* MATCHR_UTILS_H */
