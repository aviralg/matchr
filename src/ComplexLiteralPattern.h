#ifndef MATCHR_COMPLEX_LITERAL_PATTERN_H
#define MATCHR_COMPLEX_LITERAL_PATTERN_H

#include "LiteralPattern.h"
#include <R_ext/Complex.h>

namespace matchr {

class ComplexLiteralPattern: public LiteralPattern {
  public:
    ComplexLiteralPattern(SEXP r_expression,
                          SEXP r_environment,
                          const Rcomplex& value)
        : LiteralPattern(r_expression, r_environment), value_(value) {
    }

    const Rcomplex& get_value() const {
        return value_;
    }

    Context& match_value(SEXP r_value, Context& context) const override final {
        if (TYPEOF(r_value) != CPLXSXP || LENGTH(r_value) != 1) {
            context.set_failure();
            return context;
        }

        Rcomplex value1 = get_value();
        Rcomplex value2 = COMPLEX_ELT(r_value, 0);
        context.set_status(value1.r == value2.r && value1.i == value2.i);

        return context;
    }

  private:
    const Rcomplex value_;
};

} // namespace matchr

#endif /* MATCHR_COMPLEX_LITERAL_PATTERN_H */
