#ifndef MATCHR_GROUP_PATTERN_H
#define MATCHR_GROUP_PATTERN_H

#include "Pattern.h"
#include "Context.h"

namespace matchr {

class GroupPattern: public Pattern {
  public:
    explicit GroupPattern(SEXP r_expression,
                          SEXP r_environment,
                          Pattern* pattern)
        : Pattern(r_expression, r_environment), pattern_(pattern) {
    }

    ~GroupPattern() {
        delete pattern_;
    }

    const Pattern* get_inner_pattern() const {
        return pattern_;
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        return get_inner_pattern()->match_expression(expression, context);
    }

    IdentifierNames get_identifier_names() const override final {
        return get_inner_pattern()->get_identifier_names();
    }

    static GroupPattern* create(SEXP r_expression, SEXP r_environment);

  private:
    Pattern* pattern_;
};

} // namespace matchr

#endif /* MATCHR_GROUP_PATTERN_H */
