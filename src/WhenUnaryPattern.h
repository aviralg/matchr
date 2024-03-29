#ifndef MATCHR_WHEN_UNARY_PATTERN_H
#define MATCHR_WHEN_UNARY_PATTERN_H

#include "UnaryPattern.h"
#include "Context.h"

class WhenUnaryPattern: public UnaryPattern {
  public:
    explicit WhenUnaryPattern(SEXP r_expression,
                              SEXP r_predicate,
                              Pattern* pattern)
        : UnaryPattern(r_expression, pattern), r_predicate_(r_predicate) {
    }

    SEXP get_predicate() const {
        return r_predicate_;
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        /* TODO: add env with . */
        SEXP r_result = Rf_eval(get_predicate(), r_pat_env);

        bool status = (TYPEOF(r_result) == LGLSXP) &&
                      (Rf_length(r_result) == 1) && (LOGICAL(r_result)[0] != 0);

        if (!status) {
            return Context(false);
        }

        return get_sub_pattern()->match(value, r_pat_env);
    }

  private:
    SEXP r_predicate_;
};

#endif /* MATCHR_WHEN_UNARY_PATTERN_H */
