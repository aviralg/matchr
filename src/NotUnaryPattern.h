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

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        Context clone(context);

        clone.set_success();

        clone = get_sub_pattern()->match_value(r_value, clone);

        clone.toggle_status();

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }
};

} // namespace matchr

#endif /* MATCHR_NOT_UNARY_PATTERN_H */
