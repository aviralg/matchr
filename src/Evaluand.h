#ifndef MATCHR_EVALUAND_H
#define MATCHR_EVALUAND_H

#include "Context.h"

class Evaluand {
  public:
    ~Evaluand() {
    }

    SEXP get_expression() {
        return r_expression_;
    }

    SEXP evaluate(Context context, SEXP r_eval_env) {
        SEXP r_env = context.get_bindings().as_environment(r_eval_env);
        return Rf_eval(get_expression(), r_env);
    }

    static Evaluand* create(SEXP r_expression);

  private:
    SEXP r_expression_;

    Evaluand(SEXP r_expression): r_expression_(r_expression) {
        // We don't need to preserve r_expression because the outermost match
        // expression is preserved already by matcher.
    }
};

#endif /* MATCHR_EVALUAND_H */
