#include "PredicatePattern.h"
#include <cstring>

namespace matchr {

PredicatePattern* PredicatePattern::create(SEXP r_expression,
                                           SEXP r_environment) {
    if (TYPEOF(r_expression) != LANGSXP) {
        return nullptr;
    }

    SEXP ptr = r_expression;

    const char* name = CHAR(PRINTNAME(CAR(ptr)));

    /* pattern should begin with 'predicate'  */
    if (strcmp(name, "predicate") != 0) {
        return nullptr;
    }

    ptr = CDR(ptr);

    /* TODO: error */
    if (ptr == R_NilValue) {
        return nullptr;
    }

    PredicatePattern* pattern = new PredicatePattern(CAR(ptr), r_environment);

    ptr = CDR(ptr);

    /* TODO: add error, more than 1 argument to predicate  */
    if (ptr != R_NilValue) {
        delete pattern;
        return nullptr;
    }

    return pattern;
}

} // namespace matchr
