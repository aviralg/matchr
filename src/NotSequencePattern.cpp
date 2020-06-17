#include "NotSequencePattern.h"
#include <cstring>

namespace matchr {

NotSequencePattern* NotSequencePattern::create(SEXP expression) {
    /* pattern should be a function call */
    if (TYPEOF(expression) != LANGSXP) {
        return nullptr;
    }

    const char* name = CHAR(PRINTNAME(CAR(expression)));

    /* pattern should begin with 'not'  */
    if (strcmp(name, "not") != 0) {
        return nullptr;
    }

    NotSequencePattern* not_pattern = new NotSequencePattern(expression);

    for (SEXP ptr = CDR(expression); ptr != R_NilValue; ptr = CDR(ptr)) {
        Pattern* pattern = Pattern::create(CAR(ptr));
        if (pattern == nullptr) {
            /* TODO: raise error here, or_pattern is invalid; or in the pattern
             * that is being constructed */
            delete not_pattern;
            not_pattern = nullptr;
            break;
        }
        not_pattern->add_pattern(pattern);
    }

    return not_pattern;
}

} // namespace matchr
