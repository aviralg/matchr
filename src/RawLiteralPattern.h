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

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        bool status = value.is_raw_vector() && value.has_raw_value(get_value());

        clone.set_status(status);

        return clone;
    }

  private:
    const Rbyte value_;
};

#endif /* MATCHR_RAW_LITERAL_PATTERN_H */
