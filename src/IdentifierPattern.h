#ifndef MATCHR_IDENTIFIER_PATTERN_H
#define MATCHR_IDENTIFIER_PATTERN_H

#include "Pattern.h"
#include "Context.h"

namespace matchr {

class IdentifierPattern: public Pattern {
  public:
    explicit IdentifierPattern(SEXP expression, const std::string name)
        : Pattern(expression), name_(name) {
        get_identifier_names().add(name);
    }

    const std::string& get_name() const {
        return name_;
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        if (!context.bind(get_name(), expression)) {
            context.set_failure();
        } else {
            context.set_success();
        }

        return context;
    }

    static IdentifierPattern* create(SEXP expression);

  private:
    std::string name_;
};

} // namespace matchr

#endif /* MATCHR_IDENTIFIER_PATTERN_H */