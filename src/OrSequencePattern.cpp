#include "OrSequencePattern.h"
#include <cstring>

namespace matchr {

OrSequencePattern* OrSequencePattern::create(SEXP expression) {
    /* pattern should be a function call */
    if (TYPEOF(expression) != LANGSXP) {
        return nullptr;
    }

    const char* name = CHAR(PRINTNAME(CAR(expression)));

    /* pattern should begin with 'or'  */
    if (strcmp(name, "or") != 0) {
        return nullptr;
    }

    OrSequencePattern* or_pattern = new OrSequencePattern(expression);

    IdentifierNames* identifier_names = nullptr;

    for (SEXP ptr = CDR(expression); ptr != R_NilValue; ptr = CDR(ptr)) {
        Pattern* pattern = Pattern::create(CAR(ptr));
        if (pattern == nullptr) {
            /* TODO: raise error here, or_pattern is invalid; or in the pattern
             * that is being constructed */
            delete or_pattern;
            or_pattern = nullptr;
            break;
        } else if (identifier_names == nullptr) {
            identifier_names = &pattern->get_identifier_names();
        } else if (*identifier_names != pattern->get_identifier_names()) {
            /* TODO: raise error here, or_pattern is invalid; or in the pattern
             * that is being constructed */
            delete or_pattern;
            or_pattern = nullptr;
            break;
        }

        or_pattern->add_pattern(pattern);
    }

    return or_pattern;
}

} // namespace matchr
