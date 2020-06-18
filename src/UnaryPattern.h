#ifndef MATCHR_UNARY_PATTERN_H
#define MATCHR_UNARY_PATTERN_H

#include "Pattern.h"
#include <vector>

namespace matchr {

class UnaryPattern: public Pattern {
  public:
    explicit UnaryPattern(SEXP r_expression,
                          SEXP r_environment,
                          PatternSPtr sub_pattern)
        : Pattern(r_expression, r_environment), sub_pattern_(sub_pattern) {
    }

    virtual ~UnaryPattern() {
    }

    const PatternSPtr get_sub_pattern() const {
        return sub_pattern_;
    }

    PatternSPtr get_sub_pattern() {
        return sub_pattern_;
    }

  private:
    PatternSPtr sub_pattern_;
};

} // namespace matchr

#endif /* MATCHR_UNARY_PATTERN_H */
