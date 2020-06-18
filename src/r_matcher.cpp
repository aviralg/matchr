#include "r_matcher.h"
#include "Matcher.h"

using matchr::Clause;
using matchr::ClauseSPtr;
using matchr::Matcher;
using matchr::MatcherSPtr;

SEXP r_matcher_create_matcher(SEXP r_clauses) {
    MatcherSPtr matcher = std::make_shared<Matcher>();
    int count = LENGTH(r_clauses);
    for (int index = 0; index < count; ++index) {
        ClauseSPtr clause = Clause::from_sexp(VECTOR_ELT(r_clauses, index));
        matcher->add_clause(clause);
    }
    return Matcher::to_sexp(matcher);
}

SEXP r_matcher_get_clauses(SEXP r_matcher) {
    MatcherSPtr matcher = Matcher::from_sexp(r_matcher);

    int count = matcher->get_clause_count();

    SEXP r_clauses = PROTECT(allocVector(VECSXP, count));

    for (int index = 0; index < count; ++index) {
        ClauseSPtr clause = matcher->get_clause(index);
        SEXP r_clause = Clause::to_sexp(clause);
        SET_VECTOR_ELT(r_clauses, index, r_clause);
    }

    UNPROTECT(1);

    return r_clauses;
}

SEXP r_matcher_match_value(SEXP r_matcher, SEXP r_value) {
    MatcherSPtr matcher = Matcher::from_sexp(r_matcher);
    return matcher->match_value(r_value);
}
