#ifndef MATCHR_VECTOR_VARIADIC_PATTERN_H
#define MATCHR_VECTOR_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

namespace matchr {

class VectorVariadicPattern: public VariadicPattern {
  public:
    VectorVariadicPattern(SEXP r_expression, SEXP r_environment)
        : VariadicPattern(r_expression, r_environment) {
    }

    Context& match_expression(SEXP r_expression,
                              Context& context) const override final {
        SEXPTYPE type = TYPEOF(r_expression);

        if (type != INTSXP && type != REALSXP && type != STRSXP &&
            type != LGLSXP && type != RAWSXP && type != CPLXSXP) {
            context.set_failure();
            return context;
        }

        int length = LENGTH(r_expression);

        if (get_sub_pattern_count() != length) {
            context.set_failure();
            return context;
        }

        context.set_success();

        for (int index = 0; index < length && context; ++index) {
            const PatternSPtr pattern = get_sub_pattern(index);
            SEXP r_element = PROTECT(get_element_(r_expression, index));
            pattern->match_expression(r_element, context);
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
    SEXP get_element_(SEXP r_expression, int index) const {
        switch (TYPEOF(r_expression)) {
        case INTSXP:
            return ScalarInteger(INTEGER(r_expression)[index]);
            break;

        case REALSXP:
            return ScalarReal(REAL(r_expression)[index]);
            break;

        case STRSXP:
            return mkString(CHAR(STRING_ELT(r_expression, index)));
            break;

        case LGLSXP:
            return ScalarLogical(LOGICAL(r_expression)[index]);
            break;
        }

        return R_NilValue;
    }
};

} // namespace matchr

#endif /* MATCHR_VECTOR_VARIADIC_PATTERN_H */
