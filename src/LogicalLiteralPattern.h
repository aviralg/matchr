#ifndef MATCHR_LOGICAL_LITERAL_PATTERN_H
#define MATCHR_LOGICAL_LITERAL_PATTERN_H

#include "LiteralPattern.h"

class LogicalLiteralPattern: public LiteralPattern {
  public:
    LogicalLiteralPattern(SEXP r_expression, int value)
        : LiteralPattern(r_expression), value_(value) {
    }

    int get_value() const {
        return value_;
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        bool status =
            value.is_logical_vector() && value.has_logical_value(get_value());

        clone.set_status(status);

        return clone;
    }

  private:
    const int value_;
};

#endif /* MATCHR_LOGICAL_LITERAL_PATTERN_H */
