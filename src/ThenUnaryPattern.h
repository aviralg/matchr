#ifndef MATCHR_THEN_UNARY_PATTERN_H
#define MATCHR_THEN_UNARY_PATTERN_H

#include "UnaryPattern.h"
#include "Context.h"

class ThenUnaryPattern: public UnaryPattern {
  public:
    explicit ThenUnaryPattern(SEXP r_expression,
                              Pattern* pattern,
                              SEXP r_predicate)
        : UnaryPattern(r_expression, pattern), r_predicate_(r_predicate) {
    }

    SEXP get_predicate() const {
        return r_predicate_;
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        Context context = get_sub_pattern()->match(value, r_pat_env);

        if (!context) {
            return context;
        }

        SEXP r_eval_env = context.get_bindings().as_environment(r_pat_env);

        /* TODO: add env with .*/
        SEXP r_result = Rf_eval(get_predicate(), r_eval_env);

        bool status = (TYPEOF(r_result) == LGLSXP) &&
                      (Rf_length(r_result) == 1) && (LOGICAL(r_result)[0] != 0);

        if (status) {
            return context;
        }

        return Context(false);
    }

  private:
    SEXP r_predicate_;
};

#endif /* MATCHR_THEN_UNARY_PATTERN_H */
