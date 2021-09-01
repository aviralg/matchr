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

    Context match(RValue value, SEXP r_pat_env) const override final {
        bool status = value.is_logical_vector() &&
                      value.get_logical_element(0) == get_value();

        return Context(status);
    }

  private:
    const int value_;
};

#endif /* MATCHR_LOGICAL_LITERAL_PATTERN_H */
