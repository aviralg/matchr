#ifndef MATCHR_MATCHER_H
#define MATCHR_MATCHER_H

#include "clause.h"
#include <vector>

struct matcher_impl_t;

typedef matcher_impl_t* matcher_t;

namespace matcher {

matcher_t
create(const std::vector<clause_t>& clauses, SEXP r_pat_env, SEXP r_eval_env);

void destroy(matcher_t matcher);

matcher_t parse(SEXP r_expression, SEXP r_pat_env, SEXP r_eval_env);

SEXP match(matcher_t matcher, SEXP r_value);

matcher_t from_sexp(SEXP r_matcher);

SEXP to_sexp(matcher_t matcher);

} // namespace matcher

#endif /* MATCHR_MATCHER_H */
