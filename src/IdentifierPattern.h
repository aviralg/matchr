#ifndef MATCHR_IDENTIFIER_PATTERN_H
#define MATCHR_IDENTIFIER_PATTERN_H

#include "Pattern.h"
#include "Context.h"

namespace matchr {

class IdentifierPattern: public Pattern {
  public:
    explicit IdentifierPattern(SEXP r_expression,
                               SEXP r_environment,
                               const std::string name)
        : Pattern(r_expression, r_environment), name_(name) {
    }

    const std::string& get_name() const {
        return name_;
    }

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        Context clone(context);

        clone.bind(get_name(), r_value);

        clone.set_status(true);

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames(get_name());
    }

  private:
    std::string name_;
};

} // namespace matchr

#endif /* MATCHR_IDENTIFIER_PATTERN_H */
