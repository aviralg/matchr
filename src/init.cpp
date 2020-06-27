#include "r_matchr.h"
#include "r_matcher.h"
#include "r_clause.h"
#include "r_pattern.h"
#include "r_evaluand.h"
#include "r_utilities.h"

#include <R_ext/Rdynload.h>
#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* matchr */
    {"matchr_initialize_matchr", (DL_FUNC) &r_matchr_initialize_matchr, 1},
    {"matchr_finalize_matchr", (DL_FUNC) &r_matchr_finalize_matchr, 0},

    /* matcher */
    {"matcher_create_matcher", (DL_FUNC) &r_matcher_create_matcher, 1},
    {"matcher_get_clauses", (DL_FUNC) &r_matcher_get_clauses, 1},
    {"matcher_match_value", (DL_FUNC) &r_matcher_match_value, 2},

    /* clause */
    {"clause_create_clause", (DL_FUNC) &r_clause_create_clause, 2},
    {"clause_get_pattern", (DL_FUNC) &r_clause_get_pattern, 1},
    {"clause_get_evaluand", (DL_FUNC) &r_clause_get_evaluand, 1},

    /* pattern */
    {"pattern_create_wildcard_pattern", (DL_FUNC) &r_pattern_create_wildcard_pattern, 2},
    {"pattern_create_identifier_pattern", (DL_FUNC) &r_pattern_create_identifier_pattern, 3},
    {"pattern_create_integer_literal_pattern", (DL_FUNC) &r_pattern_create_integer_literal_pattern, 3},
    {"pattern_create_real_literal_pattern", (DL_FUNC) &r_pattern_create_real_literal_pattern, 3},
    {"pattern_create_raw_literal_pattern", (DL_FUNC) &r_pattern_create_raw_literal_pattern, 3},
    {"pattern_create_character_literal_pattern", (DL_FUNC) &r_pattern_create_character_literal_pattern, 3},
    {"pattern_create_logical_literal_pattern", (DL_FUNC) &r_pattern_create_logical_literal_pattern, 3},
    {"pattern_create_complex_literal_pattern", (DL_FUNC) &r_pattern_create_complex_literal_pattern, 3},
    {"pattern_create_predicate_pattern", (DL_FUNC) &r_pattern_create_predicate_pattern, 3},
    {"pattern_create_not_unary_pattern", (DL_FUNC) &r_pattern_create_not_unary_pattern, 3},
    {"pattern_create_group_unary_pattern", (DL_FUNC) &r_pattern_create_group_unary_pattern, 3},
    {"pattern_create_range_unary_pattern", (DL_FUNC) &r_pattern_create_range_unary_pattern, 5},
    {"pattern_create_and_binary_pattern", (DL_FUNC) &r_pattern_create_and_binary_pattern, 4},
    {"pattern_create_or_binary_pattern", (DL_FUNC) &r_pattern_create_or_binary_pattern, 4},
    //{"pattern_create_vector_variadic_pattern", (DL_FUNC) &r_pattern_create_vector_variadic_pattern, 3},
    {"pattern_get_expression", (DL_FUNC) &r_pattern_get_expression, 1},
    {"pattern_get_environment", (DL_FUNC) &r_pattern_get_environment, 1},
    {"pattern_get_identifier_names", (DL_FUNC) &r_pattern_get_identifier_names, 1},

    /* evaluand */
    {"evaluand_create_evaluand", (DL_FUNC) &r_evaluand_create_evaluand, 2},
    {"evaluand_get_expression", (DL_FUNC) &r_evaluand_get_expression, 1},
    {"evaluand_get_environment", (DL_FUNC) &r_evaluand_get_environment, 1},

    /* utilities */
    {"utilities_parse_identifier", (DL_FUNC) &r_utilities_parse_identifier, 1},

    {NULL, NULL, 0}};

void R_init_matchr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}

