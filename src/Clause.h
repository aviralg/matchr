#ifndef MATCHR_CLAUSE_H
#define MATCHR_CLAUSE_H

#include "Pattern.h"
#include "Evaluand.h"

class Clause {
  public:
    ~Clause() {
        delete pattern_;
        delete evaluand_;
    }

    Pattern* get_pattern() {
        return pattern_;
    }

    Evaluand* get_evaluand() {
        return evaluand_;
    }

    SEXP match(SEXP r_value, SEXP r_pat_env, SEXP r_eval_env) {
        SEXP r_result = NULL;

        Pattern* pattern = get_pattern();
        Context context = pattern->match(RValue(r_value), r_pat_env);

        if (context) {
            Evaluand* evaluand = get_evaluand();
            r_result = evaluand->evaluate(context, r_eval_env);
        }

        return r_result;
    }

    static Clause* create(SEXP r_expression);

  private:
    Pattern* pattern_;
    Evaluand* evaluand_;

    Clause(Pattern* pattern, Evaluand* evaluand)
        : pattern_(pattern), evaluand_(evaluand) {
    }
};

#endif /* MATCHR_CLAUSE_H */
