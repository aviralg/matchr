#ifndef MATCHR_AND_SEQUENCE_PATTERN_H
#define MATCHR_AND_SEQUENCE_PATTERN_H

#include "SequencePattern.h"

namespace matchr {

class AndSequencePattern: public SequencePattern {
  public:
    AndSequencePattern(SEXP expression): SequencePattern(expression) {
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override {
        for (int index = 0; index < get_pattern_count() && context; ++index) {
            const Pattern* pattern = get_pattern(index);
            pattern->match_expression(expression, context);
        }

        return context;
    }

    static AndSequencePattern* create(SEXP expression);
};

} // namespace matchr

#endif /* MATCHR_AND_SEQUENCE_PATTERN_H */
