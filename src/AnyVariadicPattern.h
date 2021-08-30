#ifndef MATCHR_ANY_VARIADIC_PATTERN_H
#define MATCHR_ANY_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

class AnyVariadicPattern: public VariadicPattern {
  public:
    AnyVariadicPattern(SEXP r_expression, const std::vector<Pattern*>& patterns)
        : VariadicPattern(r_expression, patterns) {
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        clone.set_failure();

        for (int i = 0; !clone && i < get_size(); ++i) {
            Pattern* pat = get_pattern(i);
            clone = pat->match(value, r_pat_env, context);
        }

        return clone;
    }
};

#endif /* MATCHR_ANY_VARIADIC_PATTERN_H */
