#ifndef MATCHR_RANGE_UNARY_PATTERN_H
#define MATCHR_RANGE_UNARY_PATTERN_H

#include "Pattern.h"
#include "Context.h"
#include <unordered_map>

class RangeUnaryPattern: public UnaryPattern {
  public:
    explicit RangeUnaryPattern(SEXP r_expression,
                               Pattern* sub_pattern,
                               int minimum,
                               int maximum)
        : UnaryPattern(r_expression, sub_pattern) {
        set_range(Range(minimum, maximum));
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        int length = value.get_length();

        std::vector<Context> results;
        results.reserve(length);

        const Pattern* pattern = get_sub_pattern();

        for (int i = 0; i < length; ++i) {
            RValue new_value = value.subset(i, 1);
            Context result = pattern->match(new_value, r_pat_env, context);

            if (!result) {
                return result;
            }

            results.push_back(result);
        }

        Context result(context);
        result.set_success();
        return result;
    }
};

#endif /* MATCHR_RANGE_UNARY_PATTERN_H */
