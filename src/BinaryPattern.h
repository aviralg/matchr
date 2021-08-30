#ifndef MATCHR_BINARY_PATTERN_H
#define MATCHR_BINARY_PATTERN_H

#include "Pattern.h"
#include <vector>

class BinaryPattern: public Pattern {
  public:
    explicit BinaryPattern(SEXP r_expression,
                           Pattern* first_sub_pattern,
                           Pattern* second_sub_pattern)
        : Pattern(r_expression)
        , first_sub_pattern_(first_sub_pattern)
        , second_sub_pattern_(second_sub_pattern) {
    }

    virtual ~BinaryPattern() {
    }

    const Pattern* get_first_sub_pattern() const {
        return first_sub_pattern_;
    }

    Pattern* get_first_sub_pattern() {
        return first_sub_pattern_;
    }

    const Pattern* get_second_sub_pattern() const {
        return second_sub_pattern_;
    }

    Pattern* get_second_sub_pattern() {
        return second_sub_pattern_;
    }

    IdentifierNames get_identifier_names() const override final {
        IdentifierNames names(get_first_sub_pattern()->get_identifier_names());
        names.merge(get_second_sub_pattern()->get_identifier_names());
        return names;
    }

  private:
    Pattern* first_sub_pattern_;
    Pattern* second_sub_pattern_;
};

#endif /* MATCHR_BINARY_PATTERN_H */
