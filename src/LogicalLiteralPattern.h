#ifndef MATCHR_LOGICAL_LITERAL_PATTERN_H
#define MATCHR_LOGICAL_LITERAL_PATTERN_H

#include "LiteralPattern.h"

namespace matchr {

class LogicalLiteralPattern: public LiteralPattern {
  public:
    LogicalLiteralPattern(SEXP r_expression, SEXP r_environment, int value)
        : LiteralPattern(r_expression, r_environment), value_(value) {
    }

    int get_value() const {
        return value_;
    }

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        Context clone(context);

        if (TYPEOF(r_value) != LGLSXP || LENGTH(r_value) != 1) {
            clone.set_failure();
            return clone;
        }

        clone.set_status(asLogical(r_value) == get_value());

        return clone;
    }

  private:
    const int value_;
};

} // namespace matchr

#endif /* MATCHR_LOGICAL_LITERAL_PATTERN_H */
