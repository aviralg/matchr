#ifndef MATCHR_OR_SEQUENCE_PATTERN_H
#define MATCHR_OR_SEQUENCE_PATTERN_H

#include "SequencePattern.h"

namespace matchr {

class OrSequencePattern: public SequencePattern {
  public:
    OrSequencePattern(SEXP r_expression, SEXP r_environment)
        : SequencePattern(r_expression, r_environment) {
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        context.set_failure();

        for (int index = 0; index < get_pattern_count(); ++index) {
            Context clone = context;
            const Pattern* pattern = get_pattern(index);
            pattern->match_expression(expression, clone);

            if (clone) {
                context = clone;
                break;
            }
        }

        return context;
    }

    void add_pattern(Pattern* pattern) override final {
        if (get_pattern_count() == 0) {
            get_identifier_names() = pattern->get_identifier_names();
        }
        SequencePattern::add_pattern(pattern);
    }

    static OrSequencePattern* create(SEXP r_expression, SEXP r_environment);
};

} // namespace matchr

#endif /* MATCHR_OR_SEQUENCE_PATTERN_H */
