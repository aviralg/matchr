#include "r_matcher.h"
#include "r_clause.h"
#include "r_pattern.h"
#include "r_evaluand.h"

#include <R_ext/Rdynload.h>
#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* matcher */
    {"matcher_create_matcher", (DL_FUNC) &r_matcher_create_matcher, 1},
    {"matcher_get_clauses", (DL_FUNC) &r_matcher_get_clauses, 1},
    {"matcher_match_expression", (DL_FUNC) &r_matcher_match_expression, 2},

    /* clause */
    {"clause_create_clause", (DL_FUNC) &r_clause_create_clause, 2},
    {"clause_get_pattern", (DL_FUNC) &r_clause_get_pattern, 1},
    {"clause_get_evaluand", (DL_FUNC) &r_clause_get_evaluand, 1},

    /* pattern */
    {"pattern_create_pattern", (DL_FUNC) &r_pattern_create_pattern, 1},
    {"pattern_get_expression", (DL_FUNC) &r_pattern_get_expression, 1},

    /* evaluand */
    {"evaluand_create_evaluand", (DL_FUNC) &r_evaluand_create_evaluand, 2},
    {"evaluand_get_expression", (DL_FUNC) &r_evaluand_get_expression, 1},
    {"evaluand_get_environment", (DL_FUNC) &r_evaluand_get_environment, 1},

    {NULL, NULL, 0}};

void R_init_matchr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}

