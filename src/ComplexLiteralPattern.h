#ifndef MATCHR_COMPLEX_LITERAL_PATTERN_H
#define MATCHR_COMPLEX_LITERAL_PATTERN_H

#include "LiteralPattern.h"
#include "r.h"

class ComplexLiteralPattern: public LiteralPattern {
  public:
    ComplexLiteralPattern(SEXP r_expression, const Rcomplex& value)
        : LiteralPattern(r_expression), value_(value) {
    }

    const Rcomplex& get_value() const {
        return value_;
    }

    bool is_equal(const Rcomplex& rhs) const {
        const Rcomplex& lhs = get_value();
        return lhs.r == rhs.r && lhs.i == rhs.i;
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        bool status =
            value.is_complex_vector() && value.has_complex_value(get_value());

        clone.set_status(status);

        return clone;
    }

  private:
    const Rcomplex value_;
};

#endif /* MATCHR_COMPLEX_LITERAL_PATTERN_H */
