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

    Context match_value(SEXP r_value,
                        const Context& context) const override final {
        Context clone(context);

        SEXP r_predicate = get_predicate();
        SEXP r_environment = get_environment();
        SEXP r_result = Rf_eval(Rf_lang2(r_predicate, r_value), r_environment);

        bool status = TYPEOF(r_result) == LGLSXP && LENGTH(r_result) == 1 &&
                      asLogical(r_result);

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
