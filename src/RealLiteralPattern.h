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

    Context match(RValue value, SEXP r_pat_env) const override final {
        bool status =
            value.is_real_vector() && value.get_real_element(0) == get_value();

        return Context(status);
    }

  private:
    const double value_;
};

#endif /* MATCHR_REAL_LITERAL_PATTERN_H */
