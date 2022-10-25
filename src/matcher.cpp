#include "matcher.h"
#include <cstdlib>
#include <iostream>

/********************************************************************************
 * RANGE
 *******************************************************************************/

range_t range_create(int min, int max) {
    return range_t{min, max};
}

/********************************************************************************
 * PATTERN
 *******************************************************************************/

pattern_t pattern_create(pattern_type_t type) {
    pattern_t pattern = new pattern_impl_t();
    pattern->type = type;
    pattern->str_val = nullptr;
    return pattern;
}

void pattern_destroy(pattern_t pattern) {
    if (pattern->str_val != nullptr) {
        std::free((void*) (pattern->str_val));
    }
    delete pattern;
}

range_t pattern_range(pattern_t pattern) {
    switch (pattern->type) {
    case pattern_type_t::ID:
    case pattern_type_t::LGLVAL:
    case pattern_type_t::INTVAL:
    case pattern_type_t::DBLVAL:
    case pattern_type_t::CPXVAL:
    case pattern_type_t::STRVAL:
    case pattern_type_t::RAWVAL:
    case pattern_type_t::REIM:
    case pattern_type_t::NA_POLY:
        return range_create(1, 1);
    case pattern_type_t::ANY:
    case pattern_type_t::ALL:
    case pattern_type_t::NONE:
    case pattern_type_t::LGLVEC:
    case pattern_type_t::INTVEC:
    case pattern_type_t::DBLVEC:
    case pattern_type_t::CPXVEC:
    case pattern_type_t::STRVEC:
    case pattern_type_t::RAWVEC: {
        int lo = INT_MAX;
        int hi = 0;
        for (int i = 0; i < pattern_size(pattern); ++i) {
            range_t range = pattern_range(pattern_at(pattern, i));
            lo = std::min(lo, range.min);
            hi = std::max(hi, range.max);
        }

        if (lo > hi) {
            lo = hi = 0;
        }

        return range_create(lo, hi);
    }
    case pattern_type_t::RANGE:
        return range_create(pattern->min, pattern->max);
    }
}

int pattern_size(pattern_t pattern) {
    return pattern->patterns.size();
}

pattern_t pattern_at(pattern_t pattern, int index) {
    return pattern->patterns.at(index);
}

void add_id(std::vector<std::string>& ids, const std::string& id) {
    if (id == ".") {
        return;
    }

    for (int i = 0; i < ids.size(); ++i) {
        if (ids[i] == id)
            return;
    }

    ids.push_back(id);
}

void pattern_ids_helper(const pattern_t pattern,
                        std::vector<std::string>& ids) {
    switch (pattern->type) {
    case pattern_type_t::ID:
        add_id(ids, pattern->id);
    case pattern_type_t::LGLVAL:
    case pattern_type_t::INTVAL:
    case pattern_type_t::DBLVAL:
    case pattern_type_t::CPXVAL:
    case pattern_type_t::STRVAL:
    case pattern_type_t::RAWVAL:
    case pattern_type_t::NA_POLY:
        return;
    case pattern_type_t::REIM:
    case pattern_type_t::ANY:
    case pattern_type_t::ALL:
    case pattern_type_t::NONE:
    case pattern_type_t::LGLVEC:
    case pattern_type_t::INTVEC:
    case pattern_type_t::DBLVEC:
    case pattern_type_t::CPXVEC:
    case pattern_type_t::STRVEC:
    case pattern_type_t::RAWVEC:
    case pattern_type_t::RANGE: {
        for (const pattern_t subpat: pattern->patterns) {
            pattern_ids_helper(subpat, ids);
        }
    }
    }
}

std::vector<std::string> pattern_ids(pattern_t pattern) {
    std::vector<std::string> ids;
    pattern_ids_helper(pattern, ids);
    return ids;
}

/********************************************************************************
 * CLAUSE
 *******************************************************************************/

clause_t clause_create(pattern_t pattern, SEXP r_expr) {
    clause_t clause = new clause_impl_t();
    clause->pattern = pattern;
    clause->r_expr = r_expr;
    R_PreserveObject(r_expr);
    return clause;
}

void clause_destroy(clause_t clause) {
    pattern_destroy(clause->pattern);
    R_ReleaseObject(clause->r_expr);
    delete clause;
}

/********************************************************************************
 * MATCHER
 *******************************************************************************/

matcher_t matcher_create(const std::vector<clause_t>& clauses,
                         SEXP r_pat_env,
                         SEXP r_eval_env) {
    matcher_t matcher = new matcher_impl_t();
    matcher->clauses = clauses;

    matcher->r_pat_env = r_pat_env;
    R_PreserveObject(r_pat_env);

    matcher->r_eval_env = r_eval_env;
    R_PreserveObject(r_eval_env);
    return matcher;
}

void matcher_destroy(matcher_t matcher) {
    for (int index = 0; index < matcher->clauses.size(); ++index) {
        clause_destroy(matcher->clauses[index]);
    }

    R_ReleaseObject(matcher->r_pat_env);
    R_ReleaseObject(matcher->r_eval_env);

    delete matcher;
}

matcher_t matcher_unwrap(SEXP r_matcher) {
    void* matcher = R_ExternalPtrAddr(r_matcher);
    if (matcher == NULL) {
        Rf_errorcall(R_NilValue, "null matcher object encountered");
    } else {
        return static_cast<matcher_t>(matcher);
    }
}

void destroy_sexp(SEXP r_matcher) {
    void* matcher = R_ExternalPtrAddr(r_matcher);
    if (matcher != nullptr) {
        matcher_destroy(static_cast<matcher_t>(matcher));
        R_SetExternalPtrAddr(r_matcher, nullptr);
    }
}

SEXP matcher_wrap(matcher_t matcher) {
    SEXP r_matcher =
        PROTECT(R_MakeExternalPtr(matcher, R_NilValue, R_NilValue));
    R_RegisterCFinalizerEx(r_matcher, destroy_sexp, TRUE);

    SEXP r_class = PROTECT(allocVector(STRSXP, 1));
    SET_STRING_ELT(r_class, 0, mkChar("matchr_matcher"));

    setAttrib(r_matcher, R_ClassSymbol, r_class);

    UNPROTECT(2);

    return r_matcher;
}
