#ifndef MATCHR_STRING_LITERAL_PATTERN_H
#define MATCHR_STRING_LITERAL_PATTERN_H

#include "LiteralPattern.h"

namespace matchr {

class StringLiteralPattern: public LiteralPattern {
  public:
    StringLiteralPattern(SEXP r_expression,
                         SEXP r_environment,
                         const std::string& value)
        : LiteralPattern(r_expression, r_environment), value_(value) {
    }

    const std::string& get_value() const {
        return value_;
    }

    Context& match_expression(SEXP r_expression,
                              Context& context) const override final {
        if (TYPEOF(r_expression) != STRSXP || LENGTH(r_expression) != 1) {
            context.set_failure();
            return context;
        }

        context.set_status(get_value() == CHAR(asChar(r_expression)));

        return context;
    }

  private:
    const std::string value_;
};

} // namespace matchr

#endif /* MATCHR_STRING_LITERAL_PATTERN_H */
