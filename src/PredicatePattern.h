#ifndef MATCHR_PREDICATE_PATTERN_H
#define MATCHR_PREDICATE_PATTERN_H

#include "Pattern.h"

namespace matchr {

class PredicatePattern: public Pattern {
  public:
    PredicatePattern(SEXP r_expression, SEXP r_environment, SEXP r_predicate)
        : Pattern(r_expression, r_environment), r_predicate_(r_predicate) {
    }

    SEXP get_predicate() const {
        return r_predicate_;
    }

    Context match_value(RValue value,
                        const Context& context) const override final {
        Context clone(context);

        SEXP r_predicate = get_predicate();
        SEXP r_environment = get_environment();
        RValue result =
            Rf_eval(Rf_lang2(r_predicate, value.get_value()), r_environment);

        bool status =
            result.is_logical_literal() && result.get_logical_element(0);

        clone.set_status(status);

        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }

  private:
    SEXP r_predicate_;
};

} // namespace matchr

#endif /* MATCHR_PREDICATE_PATTERN_H */
