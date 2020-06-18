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

    Context& match_expression(SEXP r_expression,
                              Context& context) const override final {
        if (TYPEOF(r_expression) != LGLSXP || LENGTH(r_expression) != 1) {
            context.set_failure();
            return context;
        }

        context.set_status(asLogical(r_expression) == get_value());

        return context;
    }

  private:
    const int value_;
};

} // namespace matchr

#endif /* MATCHR_LOGICAL_LITERAL_PATTERN_H */