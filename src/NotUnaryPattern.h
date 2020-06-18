#ifndef MATCHR_NOT_UNARY_PATTERN_H
#define MATCHR_NOT_UNARY_PATTERN_H

#include "UnaryPattern.h"

namespace matchr {

class NotUnaryPattern: public UnaryPattern {
  public:
    NotUnaryPattern(SEXP r_expression,
                    SEXP r_environment,
                    PatternSPtr sub_pattern)
        : UnaryPattern(r_expression, r_environment, sub_pattern) {
    }

    Context& match_value(SEXP r_value, Context& context) const override final {
        context.set_success();

        get_sub_pattern()->match_value(r_value, context);

        context.toggle_status();

        return context;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }
};

} // namespace matchr

#endif /* MATCHR_NOT_UNARY_PATTERN_H */
