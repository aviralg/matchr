#ifndef MATCHR_PREDICATE_PATTERN_H
#define MATCHR_PREDICATE_PATTERN_H

#include "Pattern.h"

class PredicatePattern: public Pattern {
  public:
    PredicatePattern(SEXP r_expression, SEXP r_predicate)
        : Pattern(r_expression), r_predicate_(r_predicate) {
    }

    SEXP get_predicate() const {
        return r_predicate_;
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        SEXP r_predicate = get_predicate();

        RValue result =
            Rf_eval(Rf_lang2(r_predicate, value.get_value()), r_pat_env);

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

#endif /* MATCHR_PREDICATE_PATTERN_H */
