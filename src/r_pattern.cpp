#include "r_pattern.h"
#include "Pattern.h"

using matchr::Pattern;
using matchr::PatternSPtr;

SEXP r_pattern_create_pattern(SEXP r_expression) {
    PatternSPtr pattern = std::make_shared<Pattern>(r_expression);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_get_expression(SEXP r_pattern) {
    PatternSPtr pattern = Pattern::from_sexp(r_pattern);
    return pattern->get_expression();
}
