#ifndef MATCHR_UNARY_PATTERN_H
#define MATCHR_UNARY_PATTERN_H

#include "Pattern.h"
#include <vector>

class UnaryPattern: public Pattern {
  public:
    explicit UnaryPattern(SEXP r_expression, Pattern* sub_pattern)
        : Pattern(r_expression), sub_pattern_(sub_pattern) {
    }

    virtual ~UnaryPattern() {
        delete sub_pattern_;
    }

    const Pattern* get_sub_pattern() const {
        return sub_pattern_;
    }

    Pattern* get_sub_pattern() {
        return sub_pattern_;
    }

    IdentifierNames get_identifier_names() const override final {
        return get_sub_pattern()->get_identifier_names();
    }

  private:
    Pattern* sub_pattern_;
};

#endif /* MATCHR_UNARY_PATTERN_H */
