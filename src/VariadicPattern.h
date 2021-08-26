#ifndef MATCHR_VARIADIC_PATTERN_H
#define MATCHR_VARIADIC_PATTERN_H

#include "Pattern.h"
#include <vector>

class VariadicPattern: public Pattern {
  public:
    explicit VariadicPattern(SEXP r_expression,
                             const std::vector<Pattern*>& patterns)
        : Pattern(r_expression), patterns_(patterns) {
    }

    virtual ~VariadicPattern() {
        for (int i = 0; i < get_size(); ++i) {
            delete at(i);
        }
    }

    int get_size() const {
        return patterns_.size();
    }

    Pattern* at(int index) const {
        return patterns_.at(index);
    }

    IdentifierNames get_identifier_names() const override final {
        IdentifierNames names;
        for (int i = 0; i < get_size(); ++i) {
            names.merge(at(i)->get_identifier_names());
        }
        return names;
    }

  private:
    std::vector<Pattern*> patterns_;
};

#endif /* MATCHR_VARIADIC_PATTERN_H */
