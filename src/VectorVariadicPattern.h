#ifndef MATCHR_VECTOR_VARIADIC_PATTERN_H
#define MATCHR_VECTOR_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

namespace matchr {

class VectorVariadicPattern: public VariadicPattern {
  public:
    VectorVariadicPattern(SEXP r_expression, SEXP r_environment)
        : VariadicPattern(r_expression, r_environment) {
    }

    Context& match_value(SEXP r_value, Context& context) const override final {
        SEXPTYPE type = TYPEOF(r_value);

        if (type != INTSXP && type != REALSXP && type != STRSXP &&
            type != LGLSXP && type != RAWSXP && type != CPLXSXP) {
            context.set_failure();
            return context;
        }

        int length = LENGTH(r_value);

        if (get_sub_pattern_count() != length) {
            context.set_failure();
            return context;
        }

        context.set_success();

        for (int index = 0; index < length && context; ++index) {
            const PatternSPtr pattern = get_sub_pattern(index);
            SEXP r_element = PROTECT(get_element_(r_value, index));
            pattern->match_value(r_element, context);
            UNPROTECT(1);
        }

        return context;
    }

    IdentifierNames get_identifier_names() const override final {
        IdentifierNames identifier_names;
        for (int index = 0; index < get_sub_pattern_count(); ++index) {
            const PatternSPtr pattern = get_sub_pattern(index);
            identifier_names.merge(pattern->get_identifier_names());
        }
        return identifier_names;
    }

  private:
    SEXP get_element_(SEXP r_value, int index) const {
        switch (TYPEOF(r_value)) {
        case INTSXP:
            return ScalarInteger(INTEGER(r_value)[index]);
            break;

        case REALSXP:
            return ScalarReal(REAL(r_value)[index]);
            break;

        case STRSXP:
            return mkString(CHAR(STRING_ELT(r_value, index)));
            break;

        case LGLSXP:
            return ScalarLogical(LOGICAL(r_value)[index]);
            break;
        }

        return R_NilValue;
    }
};

} // namespace matchr

#endif /* MATCHR_VECTOR_VARIADIC_PATTERN_H */
