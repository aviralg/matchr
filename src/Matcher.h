#ifndef MATCHR_MATCHER_H
#define MATCHR_MATCHER_H

#include "Clause.h"
#include <vector>

class Matcher {
  public:
    ~Matcher() {
        for (int index = 0; index < clauses_.size(); ++index) {
            Clause* clause = clauses_[index];
            delete clause;
        }

        R_ReleaseObject(r_pat_env_);
        R_ReleaseObject(r_eval_env_);
    }

    SEXP get_pattern_environment() {
        return r_pat_env_;
    }

    SEXP get_eval_environment() {
        return r_eval_env_;
    }

    SEXP match(SEXP r_value) {
        for (int index = 0; index < clauses_.size(); ++index) {
            Clause* clause = clauses_[index];
            SEXP r_result = clause->match(
                r_value, get_pattern_environment(), get_eval_environment());

            if (r_result != NULL) {
                return r_result;
            }
        }

        /* this should not happen since the last pattern is added to raise this
         * error explicitly */
        Rf_error("value did not match any pattern");
        return R_NilValue;
    }

    static Matcher* create(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env);

    static Matcher* from_sexp(SEXP r_matcher);

    static SEXP to_sexp(Matcher* matcher);

  private:
    std::vector<Clause*> clauses_;

    explicit Matcher(SEXP r_pat_env, SEXP r_eval_env)
        : r_pat_env_(r_pat_env), r_eval_env_(r_eval_env) {
        R_PreserveObject(r_pat_env_);
        R_PreserveObject(r_eval_env_);
    }

    void add_clause_(Clause* clause) {
        clauses_.push_back(clause);
    }

    SEXP r_pat_env_;
    SEXP r_eval_env_;
};

#endif /* MATCHR_MATCHER_H */
