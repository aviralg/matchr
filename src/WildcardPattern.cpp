#include "WildcardPattern.h"
#include <cstring>

namespace matchr {

WildcardPattern* WildcardPattern::create(SEXP r_expression,
                                         SEXP r_environment) {
    if (TYPEOF(r_expression) != SYMSXP) {
        return nullptr;
    }

    if (strcmp(CHAR(PRINTNAME(r_expression)), ".") != 0) {
        return nullptr;
    }

    return new WildcardPattern(r_expression, r_environment);
}

} // namespace matchr
