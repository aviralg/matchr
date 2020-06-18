#ifndef MATCHR_CLAUSE_H
#define MATCHR_CLAUSE_H

#include "Object.h"
#include "Pattern.h"
#include "Evaluand.h"

namespace matchr {

class Clause: public Object {
  public:
    explicit Clause(PatternSPtr pattern, EvaluandSPtr evaluand)
        : Object(), pattern_(pattern), evaluand_(evaluand) {
    }

    ~Clause() {
    }

    PatternSPtr get_pattern() {
        return pattern_;
    }

    EvaluandSPtr get_evaluand() {
        return evaluand_;
    }

    Context match_value(SEXP r_value) {
        return get_pattern()->match_value(r_value);
    }

    SEXP evaluate_expression(Context context) {
        return get_evaluand()->evaluate(context);
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Clause> from_sexp(SEXP r_clause);

    static SEXP to_sexp(std::shared_ptr<Clause> clause);

    static void destroy_sexp(SEXP r_clause);

  private:
    PatternSPtr pattern_;
    EvaluandSPtr evaluand_;

    static SEXP class_;
};

using ClauseSPtr = std::shared_ptr<Clause>;

} // namespace matchr

#endif /* MATCHR_CLAUSE_H */
