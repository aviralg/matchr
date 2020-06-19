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

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        Context clone(context);

        bool status =
            TYPEOF(r_value) == STRSXP &&
            get_value() ==
                CHAR(STRING_ELT(r_value, clone.get_range().get_minimum()));

        clone.set_status(status);

        return clone;
    }

  private:
    const std::string value_;
};

} // namespace matchr

#endif /* MATCHR_STRING_LITERAL_PATTERN_H */
