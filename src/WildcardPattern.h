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

    Context match_value(RValue value,
                        const Context& context) const override final {
        Context clone(context);

        clone.set_success();

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }
};

} // namespace matchr

#endif /* MATCHR_WILDCARD_PATTERN_H */
