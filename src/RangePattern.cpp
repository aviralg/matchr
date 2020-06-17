#include "RangePattern.h"
#include <limits>
#include <cstring>

namespace matchr {

RangePattern* RangePattern::create(SEXP r_expression, SEXP r_environment) {
    if (TYPEOF(r_expression) != LANGSXP) {
        return nullptr;
    }

    SEXP ptr = r_expression;

    const char* name = CHAR(PRINTNAME(CAR(ptr)));

    /* pattern should begin with 'range'  */
    if (strcmp(name, "range") != 0) {
        return nullptr;
    }

    ptr = CDR(ptr);

    /* TODO: error */
    if (ptr == R_NilValue) {
        return nullptr;
    }

    int min_value = Range::MINIMUM;
    int max_value = Range::MAXIMUM; // std::numeric_limits<int>::max();

    Pattern* sub_pattern = Pattern::create(CAR(ptr), r_environment);

    ptr = CDR(ptr);

    if (ptr != R_NilValue) {
        SEXP min_value_sexp = CAR(ptr);

        if (TYPEOF(min_value_sexp) == REALSXP && LENGTH(min_value_sexp) == 1) {
            min_value = (int) (asReal(min_value_sexp));
        } else if (TYPEOF(min_value_sexp) == INTSXP &&
                   LENGTH(min_value_sexp) == 1) {
            min_value = asInteger(min_value_sexp);
        } else if (min_value_sexp != R_MissingArg) {
            /* TODO: raise error in all cases */
            delete sub_pattern;
            return nullptr;
        }
    }

    ptr = CDR(ptr);

    if (ptr != R_NilValue) {
        SEXP max_value_sexp = CAR(ptr);

        if (TYPEOF(max_value_sexp) == REALSXP && LENGTH(max_value_sexp) == 1) {
            max_value = (int) (asReal(max_value_sexp));
        } else if (TYPEOF(max_value_sexp) == INTSXP &&
                   LENGTH(max_value_sexp) == 1) {
            max_value = asInteger(max_value_sexp);
        } else if (max_value_sexp != R_MissingArg) {
            /* TODO: raise error in all cases */
            delete sub_pattern;
            return nullptr;
        }
    }

    ptr = CDR(ptr);

    /* TODO: add error, more than 3 arguments to range  */
    if (ptr != R_NilValue) {
        delete sub_pattern;
        return nullptr;
    }

    return new RangePattern(
        r_expression, r_environment, sub_pattern, min_value, max_value);
}

} // namespace matchr
