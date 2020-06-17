#ifndef MATCHR_AND_SEQUENCE_PATTERN_H
#define MATCHR_AND_SEQUENCE_PATTERN_H

#include "SequencePattern.h"

namespace matchr {

class AndSequencePattern: public SequencePattern {
  public:
    AndSequencePattern(SEXP r_expression, SEXP r_environment)
        : SequencePattern(r_expression, r_environment) {
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        context.set_success();
        for (int index = 0; index < get_pattern_count() && context; ++index) {
            const Pattern* pattern = get_pattern(index);
            pattern->match_expression(expression, context);
        }

        return context;
    }

    IdentifierNames get_identifier_names() const override final {
        IdentifierNames identifier_names;
        for (int index = 0; index < get_pattern_count(); ++index) {
            const Pattern* pattern = get_pattern(index);
            identifier_names.merge(pattern->get_identifier_names());
        }
        return identifier_names;
    }

    static AndSequencePattern* create(SEXP r_expression, SEXP r_environment);
};

} // namespace matchr

#endif /* MATCHR_AND_SEQUENCE_PATTERN_H */
