#ifndef MATCHR_NOT_SEQUENCE_PATTERN_H
#define MATCHR_NOT_SEQUENCE_PATTERN_H

#include "SequencePattern.h"

namespace matchr {

class NotSequencePattern: public SequencePattern {
  public:
    NotSequencePattern(SEXP expression): SequencePattern(expression) {
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        context.set_success();

        for (int index = 0; index < get_pattern_count(); ++index) {
            Context clone = context;
            const Pattern* pattern = get_pattern(index);
            pattern->match_expression(expression, clone);

            if (clone) {
                context.set_failure();
                break;
            }
        }

        return context;
    }

    static NotSequencePattern* create(SEXP expression);
};

} // namespace matchr

#endif /* MATCHR_NOT_SEQUENCE_PATTERN_H */
