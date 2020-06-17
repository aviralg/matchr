#ifndef MATCHR_NOT_SEQUENCE_PATTERN_H
#define MATCHR_NOT_SEQUENCE_PATTERN_H

#include "SequencePattern.h"

namespace matchr {

class NotSequencePattern: public SequencePattern {
  public:
    NotSequencePattern(SEXP r_expression, SEXP r_environment)
        : SequencePattern(r_expression, r_environment) {
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

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }

    static NotSequencePattern* create(SEXP r_expression, SEXP r_environment);
};

} // namespace matchr

#endif /* MATCHR_NOT_SEQUENCE_PATTERN_H */
