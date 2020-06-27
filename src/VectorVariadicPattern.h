#ifndef MATCHR_VECTOR_VARIADIC_PATTERN_H
#define MATCHR_VECTOR_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

namespace matchr {

class VectorVariadicPattern: public VariadicPattern {
  public:
    VectorVariadicPattern(SEXP r_expression, SEXP r_environment)
        : VariadicPattern(r_expression, r_environment) {
    }

    Context match_value(RValue value,
                        const Context& context) const override final {
        Context clone(context);

        if (!value.is_vector()) {
            clone.set_failure();
            return clone;
        }

        int length = value.get_length();

        if (get_sub_pattern_count() != length) {
            clone.set_failure();
            return clone;
        }

        clone.set_success();

        for (int index = 0; index < length && clone; ++index) {
            const PatternSPtr pattern = get_sub_pattern(index);
            SEXP r_element = PROTECT(get_element_(r_value, index));
            clone = pattern->match_value(r_element, clone);
            UNPROTECT(1);
        }

        return clone;
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
