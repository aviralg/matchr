#ifndef MATCHR_WILDCARD_PATTERN_H
#define MATCHR_WILDCARD_PATTERN_H

#include "Pattern.h"
#include "Context.h"

class WildcardPattern: public Pattern {
  public:
    explicit WildcardPattern(SEXP r_expression): Pattern(r_expression) {
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        clone.set_success();

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }
};

#endif /* MATCHR_WILDCARD_PATTERN_H */
