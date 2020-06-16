#include "WildcardPattern.h"
#include <cstring>

namespace matchr {

WildcardPattern* WildcardPattern::create(SEXP expression) {
    if (TYPEOF(expression) != SYMSXP) {
        return nullptr;
    }

    if (strcmp(CHAR(PRINTNAME(expression)), ".") != 0) {
        return nullptr;
    }

    return new WildcardPattern(expression);
}

} // namespace matchr
