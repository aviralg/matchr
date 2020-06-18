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

    Context& match_expression(SEXP r_expression,
                              Context& context) const override final {
        context.set_success();

        SEXP r_predicate = get_predicate();
        SEXP r_environment = get_environment();
        SEXP r_result =
            Rf_eval(Rf_lang2(r_predicate, r_expression), r_environment);

        if (TYPEOF(r_result) != LGLSXP || LENGTH(r_result) != 1) {
            /* TODO: raise error  */
            context.set_failure();
            return context;
        }

        bool result = asLogical(r_result);

        context.set_status(result);

        return context;
    }

    IdentifierNames get_identifier_names() const override final {
        return IdentifierNames();
    }

  private:
    SEXP r_predicate_;
};

} // namespace matchr

#endif /* MATCHR_PREDICATE_PATTERN_H */
