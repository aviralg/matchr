#ifndef MATCHR_ALL_VARIADIC_PATTERN_H
#define MATCHR_ALL_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

class AllVariadicPattern: public VariadicPattern {
  public:
    AllVariadicPattern(SEXP r_expression, const std::vector<Pattern*>& patterns)
        : VariadicPattern(r_expression, patterns) {
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        Context context(true);

        for (int i = 0; context && i < get_size(); ++i) {
            Pattern* pat = get_pattern(i);
            context = context.merge(pat->match(value, r_pat_env));
        }

        return context;
    }
};

#endif /* MATCHR_ALL_VARIADIC_PATTERN_H */
