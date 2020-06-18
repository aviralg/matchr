#include "r_pattern.h"
#include "Pattern.h"
#include "AndBinaryPattern.h"
#include "ComplexLiteralPattern.h"
#include "GroupUnaryPattern.h"
#include "IdentifierPattern.h"
#include "IntegerLiteralPattern.h"
#include "LogicalLiteralPattern.h"
#include "NotUnaryPattern.h"
#include "OrBinaryPattern.h"
#include "PredicatePattern.h"
#include "RangeUnaryPattern.h"
#include "RawLiteralPattern.h"
#include "RealLiteralPattern.h"
#include "StringLiteralPattern.h"
#include "VectorVariadicPattern.h"
#include "WildcardPattern.h"

using matchr::AndBinaryPattern;
using matchr::ComplexLiteralPattern;
using matchr::GroupUnaryPattern;
using matchr::IdentifierPattern;
using matchr::IntegerLiteralPattern;
using matchr::LogicalLiteralPattern;
using matchr::NotUnaryPattern;
using matchr::OrBinaryPattern;
using matchr::Pattern;
using matchr::PatternSPtr;
using matchr::PredicatePattern;
using matchr::RangeUnaryPattern;
using matchr::RawLiteralPattern;
using matchr::RealLiteralPattern;
using matchr::StringLiteralPattern;
using matchr::VectorVariadicPattern;
using matchr::WildcardPattern;

SEXP r_pattern_create_wildcard_pattern(SEXP r_expression, SEXP r_environment) {
    PatternSPtr pattern =
        std::make_shared<WildcardPattern>(r_expression, r_environment);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_identifier_pattern(SEXP r_expression,
                                         SEXP r_environment,
                                         SEXP r_name) {
    std::string name(CHAR(asChar(r_name)));
    PatternSPtr pattern =
        std::make_shared<IdentifierPattern>(r_expression, r_environment, name);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_integer_literal_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_value) {
    int value = asInteger(r_value);
    PatternSPtr pattern = std::make_shared<IntegerLiteralPattern>(
        r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_real_literal_pattern(SEXP r_expression,
                                           SEXP r_environment,
                                           SEXP r_value) {
    double value = asReal(r_value);
    PatternSPtr pattern = std::make_shared<RealLiteralPattern>(
        r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_raw_literal_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_value) {
    Rbyte value = RAW_ELT(r_value, 0);
    PatternSPtr pattern =
        std::make_shared<RawLiteralPattern>(r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_string_literal_pattern(SEXP r_expression,
                                             SEXP r_environment,
                                             SEXP r_value) {
    std::string value(CHAR(asChar(r_value)));
    PatternSPtr pattern = std::make_shared<StringLiteralPattern>(
        r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_logical_literal_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_value) {
    int value = asLogical(r_value);
    PatternSPtr pattern = std::make_shared<LogicalLiteralPattern>(
        r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_complex_literal_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_value) {
    Rcomplex value = COMPLEX_ELT(r_value, 0);
    PatternSPtr pattern = std::make_shared<ComplexLiteralPattern>(
        r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_predicate_pattern(SEXP r_expression,
                                        SEXP r_environment,
                                        SEXP r_predicate_expression) {
    PatternSPtr pattern = std::make_shared<PredicatePattern>(
        r_expression, r_environment, r_predicate_expression);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_not_unary_pattern(SEXP r_expression,
                                        SEXP r_environment,
                                        SEXP r_sub_pattern) {
    PatternSPtr sub_pattern = Pattern::from_sexp(r_sub_pattern);
    PatternSPtr pattern = std::make_shared<NotUnaryPattern>(
        r_expression, r_environment, sub_pattern);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_group_unary_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_sub_pattern) {
    PatternSPtr sub_pattern = Pattern::from_sexp(r_sub_pattern);
    PatternSPtr pattern = std::make_shared<GroupUnaryPattern>(
        r_expression, r_environment, sub_pattern);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_range_unary_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_sub_pattern,
                                          SEXP r_minimum,
                                          SEXP r_maximum) {
    PatternSPtr sub_pattern = Pattern::from_sexp(r_sub_pattern);
    int minimum = asInteger(r_minimum);
    int maximum = asInteger(r_maximum);
    PatternSPtr pattern = std::make_shared<RangeUnaryPattern>(
        r_expression, r_environment, sub_pattern, minimum, maximum);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_and_binary_pattern(SEXP r_expression,
                                         SEXP r_environment,
                                         SEXP r_first_sub_pattern,
                                         SEXP r_second_sub_pattern) {
    PatternSPtr first_sub_pattern = Pattern::from_sexp(r_first_sub_pattern);
    PatternSPtr second_sub_pattern = Pattern::from_sexp(r_second_sub_pattern);
    PatternSPtr pattern = std::make_shared<AndBinaryPattern>(
        r_expression, r_environment, first_sub_pattern, second_sub_pattern);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_or_binary_pattern(SEXP r_expression,
                                        SEXP r_environment,
                                        SEXP r_first_sub_pattern,
                                        SEXP r_second_sub_pattern) {
    PatternSPtr first_sub_pattern = Pattern::from_sexp(r_first_sub_pattern);
    PatternSPtr second_sub_pattern = Pattern::from_sexp(r_second_sub_pattern);
    PatternSPtr pattern = std::make_shared<OrBinaryPattern>(
        r_expression, r_environment, first_sub_pattern, second_sub_pattern);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_vector_variadic_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_sub_patterns) {
    VectorVariadicPattern* pattern =
        new VectorVariadicPattern(r_expression, r_environment);

    int pattern_count = LENGTH(r_sub_patterns);

    for (int index = 0; index < pattern_count; ++index) {
        SEXP r_sub_pattern = VECTOR_ELT(r_sub_patterns, index);
        PatternSPtr sub_pattern = Pattern::from_sexp(r_sub_pattern);
        pattern->add_sub_pattern(sub_pattern);
    }

    return Pattern::to_sexp(std::shared_ptr<Pattern>(pattern));
}

SEXP r_pattern_get_expression(SEXP r_pattern) {
    PatternSPtr pattern = Pattern::from_sexp(r_pattern);
    return pattern->get_expression();
}

SEXP r_pattern_get_environment(SEXP r_pattern) {
    PatternSPtr pattern = Pattern::from_sexp(r_pattern);
    return pattern->get_environment();
}
