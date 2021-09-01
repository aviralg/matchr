#ifndef MATCHR_LOGICAL_UNARY_PATTERN_H
#define MATCHR_LOGICAL_UNARY_PATTERN_H

#include "UnaryPattern.h"
#include "Context.h"
#include <unordered_map>

class LogicalUnaryPattern: public UnaryPattern {
  public:
    explicit LogicalUnaryPattern(SEXP r_expression, Pattern* sub_pattern)
        : UnaryPattern(r_expression, sub_pattern) {
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        if (value.get_type() != LGLSXP) {
            return Context(false);
        }

        return get_sub_pattern()->match(value, r_pat_env);
    }
};

#endif /* MATCHR_LOGICAL_UNARY_PATTERN_H */
