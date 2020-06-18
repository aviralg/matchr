#ifndef MATCHR_GROUP_UNARY_PATTERN_H
#define MATCHR_GROUP_UNARY_PATTERN_H

#include "UnaryPattern.h"
#include "Context.h"

namespace matchr {

class GroupUnaryPattern: public UnaryPattern {
  public:
    explicit GroupUnaryPattern(SEXP r_expression,
                               SEXP r_environment,
                               PatternSPtr sub_pattern)
        : UnaryPattern(r_expression, r_environment, sub_pattern) {
    }

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        return get_sub_pattern()->match_value(r_value, context);
    }

    IdentifierNames get_identifier_names() const override final {
        return get_sub_pattern()->get_identifier_names();
    }
};

} // namespace matchr

#endif /* MATCHR_GROUP_UNARY_PATTERN_H */
