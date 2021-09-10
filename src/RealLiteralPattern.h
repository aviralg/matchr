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
        bool status = value.is_real_scalar();

        if (status) {
            double value_1 = value.get_real_element(0);
            double value_2 = get_value();

            if (std::isnan(value_1)) {
                status = std::isnan(value_2);
            }

            else if (std::isnan(value_2)) {
                status = false;
            }

            else {
                status = value_1 == value_2;
            }
        }

        return Context(status);
    }

  private:
    const double value_;
};

#endif /* MATCHR_REAL_LITERAL_PATTERN_H */
