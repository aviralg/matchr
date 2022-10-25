#ifndef MATCHR_MATCHER_H
#define MATCHR_MATCHER_H

#include "Rincludes.h"
#include <vector>
#include <string>

/********************************************************************************
 * RANGE
 *******************************************************************************/

struct range_t {
    int min;
    int max;
};

range_t range_create(int min, int max);

/********************************************************************************
 * PATTERN
 *******************************************************************************/

enum class pattern_type_t {
    ID,
    LGLVAL,
    INTVAL,
    DBLVAL,
    STRVAL,
    CPXVAL,
    RAWVAL,
    REIM,
    NA_POLY,
    ANY,
    ALL,
    NONE,
    LGLVEC,
    INTVEC,
    DBLVEC,
    STRVEC,
    CPXVEC,
    RAWVEC,
    RANGE
};

struct pattern_impl_t {
    pattern_type_t type;
    std::vector<pattern_impl_t*> patterns;
    int int_val;
    int lgl_val;
    int raw_val;
    double dbl_val;
    char* str_val;
    Rcomplex cpx_val;
    std::string id;
    int min;
    int max;
};

typedef pattern_impl_t* pattern_t;

pattern_t pattern_create(pattern_type_t type);

void pattern_destroy(pattern_t pattern);

range_t pattern_range(pattern_t pattern);

int pattern_size(pattern_t pattern);

pattern_t pattern_at(pattern_t pattern, int index);

SEXP pattern_expr(pattern_t pattern);

std::vector<std::string> pattern_ids(pattern_t pattern);

/********************************************************************************
 * CLAUSE
 *******************************************************************************/

struct clause_impl_t {
    pattern_t pattern;
    SEXP r_expr;
};

typedef clause_impl_t* clause_t;

clause_t clause_create(pattern_t pattern, SEXP r_expression);

void clause_destroy(clause_t clause);

/********************************************************************************
 * MATCHER
 *******************************************************************************/

struct matcher_impl_t {
    std::vector<clause_t> clauses;
    SEXP r_pat_env;
    SEXP r_eval_env;
};

typedef matcher_impl_t* matcher_t;

matcher_t matcher_create(const std::vector<clause_t>& clauses,
                         SEXP r_pat_env,
                         SEXP r_eval_env);

void matcher_destroy(matcher_t matcher);

matcher_t matcher_unwrap(SEXP r_matcher);

SEXP matcher_wrap(matcher_t matcher);

#endif /* MATCHR_MATCHER_H */
