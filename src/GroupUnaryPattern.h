#ifndef MATCHR_GROUP_UNARY_PATTERN_H
#define MATCHR_GROUP_UNARY_PATTERN_H

#include "UnaryPattern.h"
#include "Context.h"

class GroupUnaryPattern: public UnaryPattern {
  public:
    explicit GroupUnaryPattern(SEXP r_expression, Pattern* sub_pattern)
        : UnaryPattern(r_expression, sub_pattern) {
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        return get_sub_pattern()->match(value, r_pat_env, context);
    }
};

#endif /* MATCHR_GROUP_UNARY_PATTERN_H */
