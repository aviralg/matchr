#include "matchr.h"
#include "matcher.h"
#include <sstream>

SEXP clause_expr(clause_t clause);
SEXP pattern_seq_expr(SEXP r_fun, const std::vector<pattern_t>& patterns);
SEXP range_sym(pattern_t pattern);

SEXP r_matchr_expr(SEXP r_matcher) {
    matcher_t matcher = matcher_unwrap(r_matcher);
    SEXP r_result = R_NilValue;

    int size = matcher->clauses.size();

    for (int i = size - 1; i >= 0; --i) {
        SEXP r_clause = clause_expr(matcher->clauses[i]);
        r_result = PROTECT(CONS(r_clause, r_result));
    }

    SEXP r_fun = Rf_install("matcher");

    r_result = LCONS(r_fun, r_result);

    UNPROTECT(size);

    return r_result;
}

SEXP clause_expr(clause_t clause) {
    SEXP r_pattern = PROTECT(pattern_expr(clause->pattern));
    SEXP r_res = Rf_lang3(Rf_install("~"), r_pattern, clause->r_expr);
    UNPROTECT(2);
    return r_res;
}

SEXP pattern_expr(pattern_t pattern) {
    switch (pattern->type) {
    case pattern_type_t::ID:
        return Rf_install(pattern->id.c_str());

    case pattern_type_t::LGLVAL:
        return ScalarLogical(pattern->lgl_val);

    case pattern_type_t::INTVAL:
        return ScalarInteger(pattern->int_val);

    case pattern_type_t::DBLVAL:
        return ScalarReal(pattern->dbl_val);

    case pattern_type_t::ANY:
        return pattern_seq_expr(Rf_install("any"), pattern->patterns);

    case pattern_type_t::ALL:
        return pattern_seq_expr(Rf_install("all"), pattern->patterns);

    case pattern_type_t::NONE:
        return pattern_seq_expr(Rf_install("none"), pattern->patterns);

    case pattern_type_t::LGLVEC:
        return pattern_seq_expr(Rf_install("lgl"), pattern->patterns);

    case pattern_type_t::INTVEC:
        return pattern_seq_expr(Rf_install("int"), pattern->patterns);

    case pattern_type_t::DBLVEC:
        return pattern_seq_expr(Rf_install("dbl"), pattern->patterns);

    case pattern_type_t::RANGE:
        Rf_error("range should be handled in pattern_seq_expr");
        return R_NilValue;
    }
}

SEXP pattern_seq_expr(SEXP r_fun, const std::vector<pattern_t>& patterns) {
    SEXP r_result = R_NilValue;
    int size = patterns.size();
    int range_n = 0;

    for (int i = size - 1; i >= 0; --i) {
        pattern_t pattern = patterns[i];

        if (pattern->type == pattern_type_t::RANGE) {
            ++range_n;
            SEXP r_sym = range_sym(pattern);
            r_result = PROTECT(CONS(r_sym, r_result));
            pattern = pattern_at(pattern, 0);
        }

        SEXP r_pattern = pattern_expr(pattern);
        r_result = PROTECT(CONS(r_pattern, r_result));
    }

    r_result = LCONS(r_fun, r_result);

    UNPROTECT(size + range_n);

    return r_result;
}

SEXP range_sym(pattern_t pattern) {
    std::stringstream name;

    name << "...";

    if (pattern->min != 0) {
        name << pattern->min;
    }

    if (pattern->max != INT_MAX) {
        name << "_" << pattern->max;
    }

    const std::string& tmp = name.str();
    const char* cstr = tmp.c_str();
    return Rf_install(cstr);
}
