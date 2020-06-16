#include "r_clause.h"
#include "Clause.h"
#include "Pattern.h"
#include "Evaluand.h"

using matchr::Clause;
using matchr::ClauseSPtr;
using matchr::Evaluand;
using matchr::EvaluandSPtr;
using matchr::Pattern;
using matchr::PatternSPtr;

SEXP r_clause_create_clause(SEXP r_pattern, SEXP r_evaluand) {
    PatternSPtr pattern = Pattern::from_sexp(r_pattern);
    EvaluandSPtr evaluand = Evaluand::from_sexp(r_evaluand);
    ClauseSPtr clause = std::make_shared<Clause>(pattern, evaluand);
    return Clause::to_sexp(clause);
}

SEXP r_clause_get_pattern(SEXP r_clause) {
    ClauseSPtr clause = Clause::from_sexp(r_clause);
    PatternSPtr pattern = clause->get_pattern();
    return Pattern::to_sexp(pattern);
}

SEXP r_clause_get_evaluand(SEXP r_clause) {
    ClauseSPtr clause = Clause::from_sexp(r_clause);
    EvaluandSPtr evaluand = clause->get_evaluand();
    return Evaluand::to_sexp(evaluand);
}
