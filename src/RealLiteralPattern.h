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

    Context& match_value(SEXP r_value, Context& context) const override final {
        if (TYPEOF(r_value) != REALSXP || LENGTH(r_value) != 1) {
            context.set_failure();
            return context;
        }

        context.set_status(asReal(r_value) == get_value());

        return context;
    }

  private:
    const double value_;
};

} // namespace matchr

#endif /* MATCHR_REAL_LITERAL_PATTERN_H */
