#include "OrSequencePattern.h"
#include <cstring>

namespace matchr {

OrSequencePattern* OrSequencePattern::create(SEXP r_expression,
                                             SEXP r_environment) {
    /* pattern should be a function call */
    if (TYPEOF(r_expression) != LANGSXP) {
        return nullptr;
    }

    const char* name = CHAR(PRINTNAME(CAR(r_expression)));

    /* pattern should begin with 'or'  */
    if (strcmp(name, "or") != 0) {
        return nullptr;
    }

    OrSequencePattern* or_pattern =
        new OrSequencePattern(r_expression, r_environment);

    // IdentifierNames* identifier_names = nullptr;

    /* TODO handle identifier names */
    for (SEXP ptr = CDR(r_expression); ptr != R_NilValue; ptr = CDR(ptr)) {
        Pattern* pattern = Pattern::create(CAR(ptr), r_environment);
        if (pattern == nullptr) {
            /* TODO: raise error here, or_pattern is invalid; or in the pattern
             * that is being constructed */
            delete or_pattern;
            or_pattern = nullptr;
            break;
        }
        // else if (identifier_names == nullptr) {
        //     identifier_names = &pattern->get_identifier_names();
        // }
        // else if (*identifier_names != pattern->get_identifier_names()) {
        //     /* TODO: raise error here, or_pattern is invalid; or in the
        //     pattern
        //      * that is being constructed */
        //     delete or_pattern;
        //     or_pattern = nullptr;
        //     break;
        // }

        or_pattern->add_pattern(pattern);
    }

    return or_pattern;
}

} // namespace matchr
