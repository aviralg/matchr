#ifndef MATCHR_AND_BINARY_PATTERN_H
#define MATCHR_AND_BINARY_PATTERN_H

#include "BinaryPattern.h"

namespace matchr {

class AndBinaryPattern: public BinaryPattern {
  public:
    AndBinaryPattern(SEXP r_expression,
                     SEXP r_environment,
                     PatternSPtr first_sub_pattern,
                     PatternSPtr second_sub_pattern)
        : BinaryPattern(r_expression,
                        r_environment,
                        first_sub_pattern,
                        second_sub_pattern) {
    }

    Context match_value(RValue value,
                        const Context& context) const override final {
        Context clone(context);

        clone.set_success();

        clone = get_first_sub_pattern()->match_value(value, clone);

        if (clone) {
            clone = get_second_sub_pattern()->match_value(value, clone);
        }

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        IdentifierNames identifier_names =
            get_first_sub_pattern()->get_identifier_names();

        identifier_names.merge(get_first_sub_pattern()->get_identifier_names());

        return identifier_names;
    }
};

} // namespace matchr

#endif /* MATCHR_AND_BINARY_PATTERN_H */
