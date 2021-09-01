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

    Context match(RValue value, SEXP r_pat_env) const override final {
        SEXP r_predicate = get_predicate();

        SEXP r_result =
            Rf_eval(Rf_lang2(r_predicate, value.get_value()), r_pat_env);

        bool status = TYPEOF(r_result) == LGLSXP && Rf_length(r_result) == 1 &&
                      LOGICAL(r_result)[0] == 1;

        return Context(status);
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }

  private:
    SEXP r_predicate_;
};

#endif /* MATCHR_PREDICATE_PATTERN_H */
