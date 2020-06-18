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

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        Context clone(context);

        if (TYPEOF(r_value) != REALSXP || LENGTH(r_value) != 1) {
            clone.set_failure();
            return clone;
        }

        clone.set_status(asReal(r_value) == get_value());

        return clone;
    }

  private:
    const double value_;
};

} // namespace matchr

#endif /* MATCHR_REAL_LITERAL_PATTERN_H */
