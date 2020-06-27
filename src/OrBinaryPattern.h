#ifndef MATCHR_OR_BINARY_PATTERN_H
#define MATCHR_OR_BINARY_PATTERN_H

#include "BinaryPattern.h"

namespace matchr {

class OrBinaryPattern: public BinaryPattern {
  public:
    OrBinaryPattern(SEXP r_expression,
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
        Context clone = get_first_sub_pattern()->match_value(value, context);

        if (!clone) {
            clone = get_second_sub_pattern()->match_value(value, context);
        }

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return get_first_sub_pattern()->get_identifier_names();
    }
};

} // namespace matchr

#endif /* MATCHR_OR_BINARY_PATTERN_H */
