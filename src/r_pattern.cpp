#include "r_pattern.h"
#include "Pattern.h"

using matchr::Pattern;
using matchr::PatternSPtr;

SEXP r_pattern_create_pattern(SEXP r_expression, SEXP r_environment) {
    Pattern* pattern_ptr = Pattern::create(r_expression, r_environment);
    if (pattern_ptr == nullptr) {
        /* TODO: add call expression here  */
        Rf_errorcall(R_NilValue, "unable to create pattern");
    }
    PatternSPtr pattern = PatternSPtr(pattern_ptr);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_get_expression(SEXP r_pattern) {
    PatternSPtr pattern = Pattern::from_sexp(r_pattern);
    return pattern->get_expression();
}

SEXP r_pattern_get_environment(SEXP r_pattern) {
    PatternSPtr pattern = Pattern::from_sexp(r_pattern);
    return pattern->get_environment();
}
