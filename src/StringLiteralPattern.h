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

        if (TYPEOF(r_value) != STRSXP || LENGTH(r_value) != 1) {
            clone.set_failure();
            return clone;
        }

        clone.set_status(get_value() == CHAR(asChar(r_value)));

        return clone;
    }

  private:
    const std::string value_;
};

} // namespace matchr

#endif /* MATCHR_STRING_LITERAL_PATTERN_H */
