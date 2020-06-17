#ifndef MATCHR_RANGE_PATTERN_H
#define MATCHR_RANGE_PATTERN_H

#include "Pattern.h"
#include "Context.h"

namespace matchr {

class RangePattern: public Pattern {
  public:
    explicit RangePattern(SEXP expression,
                          Pattern* pattern,
                          int minimum,
                          int maximum)
        : Pattern(expression), pattern_(pattern) {
        get_range().set_minimum(minimum);
        get_range().set_maximum(maximum);
    }

    ~RangePattern() {
        delete pattern_;
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        context.set_success();
        return context;
    }

    static RangePattern* create(SEXP expression);

  private:
    Pattern* pattern_;
};

} // namespace matchr

#endif /* MATCHR_RANGE_PATTERN_H */
