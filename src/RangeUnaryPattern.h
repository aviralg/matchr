#ifndef MATCHR_RANGE_UNARY_PATTERN_H
#define MATCHR_RANGE_UNARY_PATTERN_H

#include "Pattern.h"
#include "Context.h"

namespace matchr {

class RangeUnaryPattern: public UnaryPattern {
  public:
    explicit RangeUnaryPattern(SEXP r_expression,
                               SEXP r_environment,
                               PatternSPtr sub_pattern,
                               int minimum,
                               int maximum)
        : UnaryPattern(r_expression, r_environment, sub_pattern) {
        get_range().set_minimum(minimum);
        get_range().set_maximum(maximum);
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        /* TODO: implement matching correctly  */
        context.set_success();
        return context;
    }

    IdentifierNames get_identifier_names() const override final {
        return get_sub_pattern()->get_identifier_names();
    }
};

} // namespace matchr

#endif /* MATCHR_RANGE_UNARY_PATTERN_H */
