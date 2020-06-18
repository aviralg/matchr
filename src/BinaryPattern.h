#ifndef MATCHR_BINARY_PATTERN_H
#define MATCHR_BINARY_PATTERN_H

#include "Pattern.h"
#include <vector>

namespace matchr {

class BinaryPattern: public Pattern {
  public:
    explicit BinaryPattern(SEXP r_expression,
                           SEXP r_environment,
                           PatternSPtr first_sub_pattern,
                           PatternSPtr second_sub_pattern)
        : Pattern(r_expression, r_environment)
        , first_sub_pattern_(first_sub_pattern)
        , second_sub_pattern_(second_sub_pattern) {
    }

    virtual ~BinaryPattern() {
    }

    const PatternSPtr get_first_sub_pattern() const {
        return first_sub_pattern_;
    }

    PatternSPtr get_first_sub_pattern() {
        return first_sub_pattern_;
    }

    const PatternSPtr get_second_sub_pattern() const {
        return second_sub_pattern_;
    }

    PatternSPtr get_second_sub_pattern() {
        return second_sub_pattern_;
    }

  private:
    PatternSPtr first_sub_pattern_;
    PatternSPtr second_sub_pattern_;
};

} // namespace matchr

#endif /* MATCHR_BINARY_PATTERN_H */
