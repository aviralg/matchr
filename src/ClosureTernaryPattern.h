#ifndef MATCHR_CLOSURE_TERNARY_PATTERN_H
#define MATCHR_CLOSURE_TERNARY_PATTERN_H

#include "TernaryPattern.h"

class ClosureTernaryPattern: public TernaryPattern {
  public:
    ClosureTernaryPattern(SEXP r_expression,
                          Pattern* formals_pattern,
                          Pattern* body_pattern,
                          Pattern* environment_pattern)
        : TernaryPattern(r_expression,
                         formals_pattern,
                         body_pattern,
                         environment_pattern) {
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        if (!(value.is_closure())) {
            return Context(false);
        }

        Context context = get_first_sub_pattern()->match(
            value.get_closure_formals(), r_pat_env);

        if (context) {
            context = context.merge(get_second_sub_pattern()->match(
                value.get_closure_body(), r_pat_env));
        }

        if (context) {
            context = context.merge(get_third_sub_pattern()->match(
                value.get_closure_environment(), r_pat_env));
        }

        return context;
    }
};

#endif /* MATCHR_CLOSURE_TERNARY_PATTERN_H */

//when
//then
//apply
