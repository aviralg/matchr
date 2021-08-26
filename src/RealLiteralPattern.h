#ifndef MATCHR_REAL_LITERAL_PATTERN_H
#define MATCHR_REAL_LITERAL_PATTERN_H

#include "LiteralPattern.h"

class RealLiteralPattern: public LiteralPattern {
  public:
    RealLiteralPattern(SEXP r_expression, double value)
        : LiteralPattern(r_expression), value_(value) {
    }

    double get_value() const {
        return value_;
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        bool status =
            value.is_real_vector() && value.has_real_value(get_value());

        clone.set_status(status);

        return clone;
    }

  private:
    const double value_;
};

#endif /* MATCHR_REAL_LITERAL_PATTERN_H */
