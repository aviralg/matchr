#ifndef MATCHR_MATCHER_H
#define MATCHR_MATCHER_H

#include "Object.h"
#include "Clause.h"
#include <vector>

namespace matchr {
class Matcher: public Object {
  public:
    explicit Matcher(): Object() {
    }

    int get_clause_count() const {
        return clauses_.size();
    }

    void add_clause(ClauseSPtr clause) {
        clauses_.push_back(clause);
    }

    ClauseSPtr get_clause(int index) {
        return clauses_[index];
    }

    SEXP match_expression(SEXP expression) {
        for (int index = 0; index < get_clause_count(); ++index) {
            ClauseSPtr clause = get_clause(index);
            Context context = clause->match_expression(expression);
            if (context) {
                return clause->evaluate_expression(context);
            }
        }
        /* TODO: add case for no matches */
        return R_NilValue;
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Matcher> from_sexp(SEXP r_matcher);

    static SEXP to_sexp(std::shared_ptr<Matcher> matcher);

    static void destroy_sexp(SEXP r_matcher);

  private:
    std::vector<ClauseSPtr> clauses_;

    static SEXP class_;
};

using MatcherSPtr = std::shared_ptr<Matcher>;

} // namespace matchr

#endif /* MATCHR_MATCHER_H */
