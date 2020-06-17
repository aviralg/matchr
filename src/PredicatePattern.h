#ifndef MATCHR_PREDICATE_PATTERN_H
#define MATCHR_PREDICATE_PATTERN_H

#include "Pattern.h"

namespace matchr {

class PredicatePattern: public Pattern {
  public:
    PredicatePattern(SEXP r_expression, SEXP r_environment)
        : Pattern(r_expression, r_environment) {
    }

    Context& match_expression(SEXP expression,
                              Context& context) const override final {
        context.set_success();

        SEXP r_predicate = get_expression();
        SEXP r_environment = get_environment();
        SEXP r_result =
            Rf_eval(Rf_lang2(r_predicate, expression), r_environment);

        if (TYPEOF(r_result) != LGLSXP || LENGTH(r_result) != 1) {
            /* TODO: raise error  */
            context.set_failure();
            return context;
        }

        bool result = asLogical(r_result);

        if (!result) {
            context.set_failure();
        }

        return context;
    }

    static PredicatePattern* create(SEXP r_expression, SEXP r_environment);
};

} // namespace matchr

#endif /* MATCHR_PREDICATE_PATTERN_H */
