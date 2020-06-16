#ifndef MATCHR_WILDCARD_PATTERN_H
#define MATCHR_WILDCARD_PATTERN_H

#include "Pattern.h"
#include "Context.h"

namespace matchr {

class WildcardPattern: public Pattern {
  public:
    explicit WildcardPattern(SEXP expression): Pattern(expression) {
    }

    Context match_expression(SEXP expression) const override final {
        Context context(true);
        return context;
    }

    static WildcardPattern* create(SEXP expression);
};

} // namespace matchr

#endif /* MATCHR_WILDCARD_PATTERN_H */
