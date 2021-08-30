#ifndef MATCHR_VECTOR_UNARY_PATTERN_H
#define MATCHR_VECTOR_UNARY_PATTERN_H

#include "UnaryPattern.h"
#include "Context.h"
#include <unordered_map>

class VectorUnaryPattern: public UnaryPattern {
  public:
    explicit VectorUnaryPattern(SEXP r_expression, Pattern* sub_pattern)
        : UnaryPattern(r_expression, sub_pattern) {
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        if (!(value.is_vector())) {
            clone.set_failure();
            return clone;
        }

        return get_sub_pattern()->match(value, r_pat_env, context);
    }
};

#endif /* MATCHR_VECTOR_UNARY_PATTERN_H */
