#ifndef MATCHR_NONE_VARIADIC_PATTERN_H
#define MATCHR_NONE_VARIADIC_PATTERN_H

#include "VariadicPattern.h"

class NoneVariadicPattern: public VariadicPattern {
  public:
    NoneVariadicPattern(SEXP r_expression,
                        const std::vector<Pattern*>& patterns)
        : VariadicPattern(r_expression, patterns) {
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        Context context(false);

        for (int i = 0; !context && i < get_size(); ++i) {
            Pattern* pat = get_pattern(i);
            context = pat->match(value, r_pat_env);
        }

        context.toggle_status();

        return context;
    }
};

#endif /* MATCHR_NONE_VARIADIC_PATTERN_H */
