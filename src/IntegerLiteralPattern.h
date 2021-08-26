#ifndef MATCHR_INTEGER_LITERAL_PATTERN_H
#define MATCHR_INTEGER_LITERAL_PATTERN_H

#include "LiteralPattern.h"

class IntegerLiteralPattern: public LiteralPattern {
  public:
    IntegerLiteralPattern(SEXP r_expression, int value)
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
            value.is_integer_vector() && value.has_integer_value(get_value());

        clone.set_status(status);

        return clone;
    }

  private:
    const int value_;
};

#endif /* MATCHR_INTEGER_LITERAL_PATTERN_H */
