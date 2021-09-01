#ifndef MATCHR_RAW_LITERAL_PATTERN_H
#define MATCHR_RAW_LITERAL_PATTERN_H

#include "LiteralPattern.h"

class RawLiteralPattern: public LiteralPattern {
  public:
    RawLiteralPattern(SEXP r_expression, Rbyte value)
        : LiteralPattern(r_expression), value_(value) {
    }

    Rbyte get_value() const {
        return value_;
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        bool status =
            value.is_raw_vector() && value.get_raw_element(0) == get_value();

        return Context(status);
    }

  private:
    const Rbyte value_;
};

#endif /* MATCHR_RAW_LITERAL_PATTERN_H */
