#ifndef MATCHR_SATISFIES_PATTERN_H
#define MATCHR_SATISFIES_PATTERN_H

#include "Pattern.h"
#include "Context.h"

class SatisfiesPattern: public Pattern {
  public:
    explicit SatisfiesPattern(SEXP r_expression, SEXP r_predicate)
        : Pattern(r_expression), r_predicate_(r_predicate) {
    }

    SEXP get_predicate() const {
        return r_predicate_;
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        SEXP r_predicate = get_predicate();

        /* TODO: add env with .*/
        SEXP r_result = Rf_eval(r_predicate, r_pat_env);

        bool status = (TYPEOF(r_result) == LGLSXP) &&
                      (Rf_length(r_result) == 1) && (LOGICAL(r_result)[0] != 0);

        return Context(status);
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }

  private:
    SEXP r_predicate_;
};

#endif /* MATCHR_SATISFIES_PATTERN_H */
