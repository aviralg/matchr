#ifndef MATCHR_CLAUSE_H
#define MATCHR_CLAUSE_H

#include "pattern.h"

struct clause_impl_t;

typedef clause_impl_t* clause_t;

namespace clause {

clause_t create(pattern_t pattern, SEXP r_expression);

clause_t parse(SEXP r_expression);

void destroy(clause_t clause);

SEXP match(clause_t clause, SEXP r_value, SEXP r_pat_env, SEXP r_eval_env);

} // namespace clause

#endif /* MATCHR_CLAUSE_H */
