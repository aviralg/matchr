#ifndef MATCHR_ALL_VARIADIC_PATTERN_H
#define MATCHR_ALL_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

class AllVariadicPattern: public VariadicPattern {
  public:
    AllVariadicPattern(SEXP r_expression, const std::vector<Pattern*>& patterns)
        : VariadicPattern(r_expression, patterns) {
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        clone.set_success();

        for (int i = 0; clone && i < get_size(); ++i) {
            Pattern* pat = at(i);
            clone = pat->match(value, r_pat_env, clone);
        }

        return clone;
    }
};

#endif /* MATCHR_ALL_VARIADIC_PATTERN_H */