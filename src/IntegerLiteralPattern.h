#ifndef MATCHR_INTEGER_LITERAL_PATTERN_H
#define MATCHR_INTEGER_LITERAL_PATTERN_H

#include "LiteralPattern.h"

namespace matchr {

class IntegerLiteralPattern: public LiteralPattern {
  public:
    IntegerLiteralPattern(SEXP r_expression, SEXP r_environment, int value)
        : LiteralPattern(r_expression, r_environment), value_(value) {
    }

    int get_value() const {
        return value_;
    }

    Context match_value(RValue value,
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

} // namespace matchr

#endif /* MATCHR_INTEGER_LITERAL_PATTERN_H */
