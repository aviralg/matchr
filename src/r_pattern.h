#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>

extern "C" {
SEXP r_pattern_create_wildcard_pattern(SEXP r_expression, SEXP r_environment);
SEXP r_pattern_create_identifier_pattern(SEXP r_expression,
                                         SEXP r_environment,
                                         SEXP r_name);
SEXP r_pattern_create_integer_literal_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_value);
SEXP r_pattern_create_real_literal_pattern(SEXP r_expression,
                                           SEXP r_environment,
                                           SEXP r_value);
SEXP r_pattern_create_raw_literal_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_value);
SEXP r_pattern_create_string_literal_pattern(SEXP r_expression,
                                             SEXP r_environment,
                                             SEXP r_value);
SEXP r_pattern_create_logical_literal_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_value);
SEXP r_pattern_create_complex_literal_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_value);
SEXP r_pattern_create_predicate_pattern(SEXP r_expression,
                                        SEXP r_environment,
                                        SEXP r_predicate_expression);
SEXP r_pattern_create_not_unary_pattern(SEXP r_expression,
                                        SEXP r_environment,
                                        SEXP r_pattern);
SEXP r_pattern_create_group_unary_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_pattern);
SEXP r_pattern_create_range_unary_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_sub_pattern,
                                          SEXP r_minimum,
                                          SEXP r_maximum);
SEXP r_pattern_create_and_binary_pattern(SEXP r_expression,
                                         SEXP r_environment,
                                         SEXP r_first_pattern,
                                         SEXP r_second_pattern);
SEXP r_pattern_create_or_binary_pattern(SEXP r_expression,
                                        SEXP r_environment,
                                        SEXP r_first_pattern,
                                        SEXP r_second_pattern);
SEXP r_pattern_create_vector_variadic_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_sub_patterns);
SEXP r_pattern_get_expression(SEXP r_pattern);
SEXP r_pattern_get_environment(SEXP r_pattern);
SEXP r_pattern_get_identifier_names(SEXP r_pattern);
}
