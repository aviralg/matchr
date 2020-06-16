#include "IdentifierPattern.h"
#include <cstring>

namespace matchr {

IdentifierPattern* IdentifierPattern::create(SEXP expression) {
    if (TYPEOF(expression) != SYMSXP) {
        return nullptr;
    }

    const char* name = CHAR(PRINTNAME(expression));

    if (strcmp(name, ".") == 0) {
        return nullptr;
    }

    return new IdentifierPattern(expression, name);
}

} // namespace matchr
