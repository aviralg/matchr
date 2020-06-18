#ifndef MATCHR_LITERAL_PATTERN_H
#define MATCHR_LITERAL_PATTERN_H

#include "Pattern.h"

namespace matchr {

class LiteralPattern: public Pattern {
  public:
    LiteralPattern(SEXP r_expression, SEXP r_environment)
        : Pattern(r_expression, r_environment) {
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }
};

} // namespace matchr

#endif /* MATCHR_LITERAL_PATTERN_H */
