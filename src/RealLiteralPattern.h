#ifndef MATCHR_REAL_LITERAL_PATTERN_H
#define MATCHR_REAL_LITERAL_PATTERN_H

#include "LiteralPattern.h"

namespace matchr {

class RealLiteralPattern: public LiteralPattern {
  public:
    RealLiteralPattern(SEXP r_expression, SEXP r_environment, double value)
        : LiteralPattern(r_expression, r_environment), value_(value) {
    }

    double get_value() const {
        return value_;
    }

    Context match_value(RValue value,
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

} // namespace matchr

#endif /* MATCHR_REAL_LITERAL_PATTERN_H */
