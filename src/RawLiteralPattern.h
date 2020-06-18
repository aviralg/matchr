#ifndef MATCHR_RAW_LITERAL_PATTERN_H
#define MATCHR_RAW_LITERAL_PATTERN_H

#include "LiteralPattern.h"

namespace matchr {

class RawLiteralPattern: public LiteralPattern {
  public:
    RawLiteralPattern(SEXP r_expression, SEXP r_environment, Rbyte value)
        : LiteralPattern(r_expression, r_environment), value_(value) {
    }

    Rbyte get_value() const {
        return value_;
    }

    Context& match_expression(SEXP r_expression,
                              Context& context) const override final {
        if (TYPEOF(r_expression) != RAWSXP && LENGTH(r_expression) != 1) {
            context.set_failure();
            return context;
        }

        context.set_status(RAW_ELT(r_expression, 0) == get_value());

        return context;
    }

  private:
    const Rbyte value_;
};

} // namespace matchr

#endif /* MATCHR_RAW_LITERAL_PATTERN_H */
