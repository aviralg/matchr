#include "AndSequencePattern.h"
#include <cstring>

namespace matchr {

AndSequencePattern* AndSequencePattern::create(SEXP r_expression,
                                               SEXP r_environment) {
    /* pattern should be a function call */
    if (TYPEOF(r_expression) != LANGSXP) {
        return nullptr;
    }

    const char* name = CHAR(PRINTNAME(CAR(r_expression)));

    /* pattern should begin with 'and'  */
    if (strcmp(name, "and") != 0) {
        return nullptr;
    }

    AndSequencePattern* and_pattern =
        new AndSequencePattern(r_expression, r_environment);

    for (SEXP ptr = CDR(r_expression); ptr != R_NilValue; ptr = CDR(ptr)) {
        Pattern* pattern = Pattern::create(CAR(ptr), r_environment);
        if (pattern == nullptr) {
            /* TODO: raise error here, and_pattern is invalid; or in the pattern
             * that is being constructed */
            delete and_pattern;
            and_pattern = nullptr;
            break;
        }
        and_pattern->add_pattern(pattern);
    }

    return and_pattern;
}

} // namespace matchr
