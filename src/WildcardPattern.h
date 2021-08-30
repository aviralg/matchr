#ifndef MATCHR_WILDCARD_PATTERN_H
#define MATCHR_WILDCARD_PATTERN_H

#include "Pattern.h"
#include "Context.h"

class WildcardPattern: public Pattern {
  public:
    explicit WildcardPattern(SEXP r_expression, const std::string name)
        : Pattern(r_expression), name_(name), dot_(name == ".") {
    }

    const std::string& get_name() const {
        return name_;
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        if (!dot_) {
            clone.get_bindings().bind(get_name(), value.get_value());
        }

        clone.set_success();

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }

  private:
    std::string name_;
    bool dot_;
};

#endif /* MATCHR_WILDCARD_PATTERN_H */
