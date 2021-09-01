#ifndef MATCHR_ANY_VARIADIC_PATTERN_H
#define MATCHR_ANY_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

class AnyVariadicPattern: public VariadicPattern {
  public:
    AnyVariadicPattern(SEXP r_expression, const std::vector<Pattern*>& patterns)
        : VariadicPattern(r_expression, patterns) {
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        Context context(false);

        for (int i = 0; i < get_size() && !context; ++i) {
            Pattern* pat = get_pattern(i);
            context = pat->match(value, r_pat_env);
        }

        return context;
    }
};

#endif /* MATCHR_ANY_VARIADIC_PATTERN_H */
