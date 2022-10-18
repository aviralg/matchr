#ifndef MATCHR_UTILITIES_H
#define MATCHR_UTILITIES_H

#include "r.h"
#include <cmath>
#include "output.h"
#include "input.h"

void initialize_utilities(SEXP r_package_environment);

SEXP new_environment(bool hash, int size, SEXP r_parent);

inline bool is_equal_char(SEXP r_char_a, SEXP r_char_b) {
    if (r_char_a == NA_STRING) {
        return (r_char_b == NA_STRING);
    }

    if (r_char_b == NA_STRING) {
        return false;
    }

    return !strcmp(CHAR(r_char_a), CHAR(r_char_b));
}

inline bool is_equal_double(double a, double b) {
    if (std::isnan(a)) {
        return std::isnan(b);
    }

    if (std::isnan(b)) {
        return false;
    }

    return a == b;
}

SEXP safe_eval(SEXP r_expr,
               SEXP r_env,
               input_t input,
               output_t output,
               int unprotect_count);

#endif /* MATCHR_UTILITIES_H */
