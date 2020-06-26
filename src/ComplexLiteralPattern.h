#ifndef MATCHR_COMPLEX_LITERAL_PATTERN_H
#define MATCHR_COMPLEX_LITERAL_PATTERN_H

#include "LiteralPattern.h"
#include "r.h"

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

    bool is_equal(const Rcomplex& rhs) const {
        const Rcomplex& lhs = get_value();
        return lhs.r == rhs.r && lhs.i == rhs.i;
    }

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        Context clone(context);

        bool status =
            TYPEOF(r_value) == CPLXSXP &&
            is_equal(COMPLEX_ELT(r_value, clone.get_range().get_minimum()));

        clone.set_status(status);

        return clone;
    }

  private:
    const Rcomplex value_;
};

} // namespace matchr

#endif /* MATCHR_COMPLEX_LITERAL_PATTERN_H */
