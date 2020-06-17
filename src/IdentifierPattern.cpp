#include "IdentifierPattern.h"
#include <cstring>

namespace matchr {

IdentifierPattern* IdentifierPattern::create(SEXP r_expression,
                                             SEXP r_environment) {
    if (TYPEOF(r_expression) != SYMSXP) {
        return nullptr;
    }

    const char* name = CHAR(PRINTNAME(r_expression));

    if (strcmp(name, ".") == 0) {
        return nullptr;
    }

    return new IdentifierPattern(r_expression, r_environment, name);
}

} // namespace matchr
