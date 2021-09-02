#ifndef MATCHR_WITH_UNARY_PATTERN_H
#define MATCHR_WITH_UNARY_PATTERN_H

#include "UnaryPattern.h"
#include "Context.h"

class WithUnaryPattern: public UnaryPattern {
  public:
    explicit WithUnaryPattern(SEXP r_expression,
                              SEXP r_generator,
                              Pattern* pattern)
        : UnaryPattern(r_expression, pattern), r_generator_(r_generator) {
    }

    SEXP get_generator() const {
        return r_generator_;
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        /* TODO: add env with . */
        SEXP r_result = Rf_eval(get_generator(), r_pat_env);

        return get_sub_pattern()->match(RValue(r_result), r_pat_env);
    }

  private:
    SEXP r_generator_;
};

#endif /* MATCHR_WITH_UNARY_PATTERN_H */
