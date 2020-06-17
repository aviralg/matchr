#ifndef MATCHR_WILDCARD_PATTERN_H
#define MATCHR_WILDCARD_PATTERN_H

#include "Pattern.h"
#include "Context.h"

namespace matchr {

class WildcardPattern: public Pattern {
  public:
    explicit WildcardPattern(SEXP r_expression, SEXP r_environment)
        : Pattern(r_expression, r_environment) {
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        context.set_success();
        return context;
    }

    static WildcardPattern* create(SEXP r_expression, SEXP r_environment);
};

} // namespace matchr

#endif /* MATCHR_WILDCARD_PATTERN_H */
