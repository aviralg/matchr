#include "GroupPattern.h"
#include <limits>
#include <cstring>

namespace matchr {

GroupPattern* GroupPattern::create(SEXP r_expression, SEXP r_environment) {
    if (TYPEOF(r_expression) != LANGSXP) {
        return nullptr;
    }

    SEXP ptr = r_expression;

    const char* name = CHAR(PRINTNAME(CAR(ptr)));

    /* pattern should begin with '('  */
    if (strcmp(name, "(") != 0) {
        return nullptr;
    }

    ptr = CDR(ptr);

    /* TODO: error */
    if (ptr == R_NilValue) {
        return nullptr;
    }

    Pattern* sub_pattern = Pattern::create(CAR(ptr), r_environment);

    ptr = CDR(ptr);

    /* TODO: add error, more than 1 argument to group  */
    if (ptr != R_NilValue) {
        delete sub_pattern;
        return nullptr;
    }

    return new GroupPattern(r_expression, r_environment, sub_pattern);
}

} // namespace matchr
