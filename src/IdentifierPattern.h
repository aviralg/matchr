#ifndef MATCHR_IDENTIFIER_PATTERN_H
#define MATCHR_IDENTIFIER_PATTERN_H

#include "Pattern.h"
#include "Context.h"

class IdentifierPattern: public Pattern {
  public:
    explicit IdentifierPattern(SEXP r_expression, const std::string name)
        : Pattern(r_expression), name_(name) {
    }

    const std::string& get_name() const {
        return name_;
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        clone.get_bindings().bind(get_name(), value.get_value());

        clone.set_status(true);

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames(get_name());
    }

  private:
    std::string name_;
};

#endif /* MATCHR_IDENTIFIER_PATTERN_H */
