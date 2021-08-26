#ifndef MATCHR_LITERAL_PATTERN_H
#define MATCHR_LITERAL_PATTERN_H

#include "Pattern.h"

class LiteralPattern: public Pattern {
  public:
    LiteralPattern(SEXP r_expression): Pattern(r_expression) {
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }
};

#endif /* MATCHR_LITERAL_PATTERN_H */
