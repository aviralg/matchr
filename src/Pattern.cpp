#include "Pattern.h"
#include "AllVariadicPattern.h"
#include "ComplexLiteralPattern.h"
#include "GroupUnaryPattern.h"
#include "IntegerLiteralPattern.h"
#include "LogicalLiteralPattern.h"
#include "NoneVariadicPattern.h"
#include "AnyVariadicPattern.h"
#include "RangeUnaryPattern.h"
#include "RawLiteralPattern.h"
#include "RealLiteralPattern.h"
#include "CharacterLiteralPattern.h"
#include "VectorUnaryPattern.h"
#include "WildcardPattern.h"
#include "SequencePattern.h"
#include "WhenUnaryPattern.h"
#include "ThenUnaryPattern.h"
#include "WithUnaryPattern.h"
#include "IntegerUnaryPattern.h"
#include "RealUnaryPattern.h"
#include "LogicalUnaryPattern.h"
#include "RawUnaryPattern.h"
#include "ListUnaryPattern.h"
#include "LanguageUnaryPattern.h"
#include "CharacterUnaryPattern.h"
#include "NullLiteralPattern.h"
#include "PairlistUnaryPattern.h"
#include "ClosureTernaryPattern.h"

class Result {
  public:
    Result(Pattern* pattern): pattern_(pattern), error_("") {
    }

    Result(const std::string& error): pattern_(nullptr), error_(error) {
    }

    bool has_pattern() const {
        return pattern_ != nullptr;
    }

    Pattern* get_pattern() {
        return pattern_;
    }

    const std::string& get_error() {
        return error_;
    }

    bool has_error() const {
        return !has_pattern();
    }

  private:
    Pattern* pattern_;
    std::string error_;
};

Result create_helper(SEXP r_expression);

Pattern* Pattern::create(SEXP r_expression) {
    Result result = create_helper(r_expression);
    if (result.has_error()) {
        Rf_error("%s", result.get_error().c_str());
    }

    return result.get_pattern();
}

template <typename T>
Result create_binary_pattern(const std::string& function_name,
                             SEXP r_expression) {
    Result left = create_helper(CADR(r_expression));

    if (left.has_error()) {
        return left;
    }

    Result right = create_helper(CADDR(r_expression));

    if (right.has_error()) {
        delete left.get_pattern();
        return right;
    }

    return Result(new T(r_expression, left.get_pattern(), right.get_pattern()));
}

template <typename T>
Result parse_ternary_pattern_sequence(const std::string& function_name,
                                      SEXP r_expression) {
    Result first = create_helper(CADR(r_expression));

    if (first.has_error()) {
        return first;
    }

    Result second = create_helper(CADDR(r_expression));

    if (second.has_error()) {
        delete first.get_pattern();
        return second;
    }

    Result third = create_helper(CADDDR(r_expression));

    if (third.has_error()) {
        delete first.get_pattern();
        delete second.get_pattern();
        return third;
    }

    return Result(new T(r_expression,
                        first.get_pattern(),
                        second.get_pattern(),
                        third.get_pattern()));
}

template <typename T>
Result create_unary_pattern(const std::string& function_name,
                            SEXP r_expression) {
    Result inner = create_helper(CADR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return Result(new T(r_expression, inner.get_pattern()));
}

Result parse_when_pattern(const std::string& function_name,
                           SEXP r_expression) {
    if (Rf_length(r_expression) != 3) {
        return Result("incorrect number of arguments passed to when");
    }

    Result inner = create_helper(CADDR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return Result(new WhenUnaryPattern(
        r_expression, CADR(r_expression), inner.get_pattern()));
}

Result parse_then_pattern(const std::string& function_name, SEXP r_expression) {
    if (Rf_length(r_expression) != 3) {
        return Result("incorrect number of arguments passed to then");
    }

    Result inner = create_helper(CADR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return Result(new ThenUnaryPattern(
        r_expression, inner.get_pattern(), CADDR(r_expression)));
}

Result parse_with_pattern(const std::string& function_name, SEXP r_expression) {
    if (Rf_length(r_expression) != 3) {
        return Result("incorrect number of arguments passed to with");
    }

    Result inner = create_helper(CADDR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return Result(new WithUnaryPattern(
        r_expression, CADR(r_expression), inner.get_pattern()));
}

template <typename T>
Result create_variadic_pattern(const std::string& function_name,
                               SEXP r_expression) {
    std::vector<Pattern*> patterns;

    for (SEXP r_tail = CDR(r_expression); r_tail != R_NilValue;
         r_tail = CDR(r_tail)) {
        SEXP r_head = CAR(r_tail);

        Result result = create_helper(r_head);

        if (result.has_error()) {
            for (int i = 0; i < patterns.size(); ++i) {
                delete patterns[i];
            }
            patterns.clear();
            return result;
        }

        patterns.push_back(result.get_pattern());
    }

    return Result(new T(r_expression, patterns));
}

std::tuple<int, int> parse_range(SEXP r_expression) {
    std::string identifier = CHAR(PRINTNAME(r_expression));

    if (identifier[0] != '.' || identifier[1] != '.' || identifier[2] != '.') {
        return {-1, -1};
    }

    int length = identifier.size();
    int i = 3;
    int min = 0;

    if (i == length) {
        return {0, 0};
    }

    for (; i < length && isdigit(identifier[i]); ++i) {
        min = min * 10 + (identifier[i] - '0');
    }

    if (i < length && identifier[i] != '_') {
        return {-1, -1};
    }

    if (i == length) {
        return {min, INT_MAX};
    }

    /* for _ */
    ++i;

    /* if the pattern is ..<min>_ */
    if (i == length) {
        return {-1, -1};
    }

    int max = 0;
    for (; i < length && isdigit(identifier[i]); ++i) {
        max = max * 10 + (identifier[i] - '0');
    }

    if (i != length) {
        return {-1, -1};
    }

    return {min, max};
}

template <typename T>
Result parse_greedy_pattern_sequence(const std::string& function_name,
                                     SEXP r_expression) {
    std::vector<Pattern*> patterns;

    bool range = false;
    for (SEXP r_tail = CDR(r_expression); r_tail != R_NilValue;
         r_tail = CDR(r_tail)) {
        SEXP r_head = CAR(r_tail);

        if (TYPEOF(r_head) == SYMSXP) {
            int min;
            int max;

            std::tie(min, max) = parse_range(r_head);

            if (min != -1 && max != -1) {
                if (!range) {
                    for (int i = 0; i < patterns.size(); ++i) {
                        delete patterns[i];
                    }
                    return Result("expected pattern, not ...");
                }

                if (min < 0 || max < 0 || max < min) {
                    for (int i = 0; i < patterns.size(); ++i) {
                        delete patterns[i];
                    }
                    return Result("invalid minimum or maximum for range");
                }

                Pattern* pattern =
                    new RangeUnaryPattern(r_head, patterns.back(), min, max);
                patterns.back() = pattern;
                range = false;
                continue;
            }
        }

        Result result = create_helper(r_head);

        if (result.has_error()) {
            for (int i = 0; i < patterns.size(); ++i) {
                delete patterns[i];
            }
            patterns.clear();
            return result;
        }

        patterns.push_back(result.get_pattern());
        range = true;
    }

    return Result(
        new T(r_expression, new SequencePattern(CDR(r_expression), patterns)));
}

template <typename T>
Result create_sequence_pattern(const std::string& function_name,
                               SEXP r_expression) {
    Result result =
        create_variadic_pattern<SequencePattern>(function_name, r_expression);

    if (result.has_error()) {
        return result;
    }

    return Result(new T(r_expression, result.get_pattern()));
}

Result create_wildcard_pattern(SEXP r_expression) {
    std::string identifier(CHAR(PRINTNAME(r_expression)));
    return Result(new WildcardPattern(r_expression, identifier));
}

Result create_range_pattern(const std::string& function_name,
                            SEXP r_expression) {
    const int length = Rf_length(r_expression);

    if (length < 2 || length > 4) {
        std::string message = "range passed invalid number of arguments";
        return Result(message);
    }

    Result inner = create_helper(CADR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    int min = 0;
    int max = INT_MAX;

    if (length >= 3) {
        SEXP r_min = CADDR(r_expression);

        if (TYPEOF(r_min) == INTSXP) {
            min = INTEGER(r_min)[0];
        } else if (TYPEOF(r_min) == REALSXP) {
            min = REAL(r_min)[0];
        } else {
            delete inner.get_pattern();
            std::string message = "invalid minimum for range";
            return Result(message);
        }
    }

    if (length == 4) {
        SEXP r_max = CADDDR(r_expression);

        if (TYPEOF(r_max) == INTSXP) {
            max = INTEGER(r_max)[0];
        } else if (TYPEOF(r_max) == REALSXP) {
            max = REAL(r_max)[0];
        } else {
            delete inner.get_pattern();
            std::string message = "invalid maximum for range";
            return Result(message);
        }
    }

    if (min < 0 || max < 0 || max < min) {
        delete inner.get_pattern();
        std::string message = "invalid minimum or maximum for range";
        return Result(message);
    }

    return Result(
        new RangeUnaryPattern(r_expression, inner.get_pattern(), min, max));
}

Result create_helper(SEXP r_expression) {
    SEXPTYPE expr_type = TYPEOF(r_expression);

    if (expr_type == SYMSXP) {
        return create_wildcard_pattern(r_expression);
    }

    else if (expr_type == LGLSXP && Rf_length(r_expression) == 1) {
        return Result(
            new LogicalLiteralPattern(r_expression, asLogical(r_expression)));
    }

    else if (expr_type == INTSXP && Rf_length(r_expression) == 1) {
        return Result(
            new IntegerLiteralPattern(r_expression, asInteger(r_expression)));
    }

    else if (expr_type == REALSXP && Rf_length(r_expression) == 1) {
        return Result(
            new RealLiteralPattern(r_expression, asReal(r_expression)));
    }

    else if (expr_type == STRSXP && Rf_length(r_expression) == 1) {
        const char* value = NULL;
        SEXP r_char = STRING_ELT(r_expression, 0);
        if (r_char != NULL) {
            value = CHAR(r_char);
        }

        return Result(new CharacterLiteralPattern(r_expression, value));
    }

    else if (expr_type == LANGSXP) {
        /* TODO: check if the CAR is a symbol or not */
        std::string function_name = CHAR(PRINTNAME(CAR(r_expression)));

        if (function_name == "(") {
            return create_unary_pattern<GroupUnaryPattern>(function_name,
                                                           r_expression);
        }

        else if (function_name == "when") {
            return parse_when_pattern(function_name, r_expression);
        }

        else if (function_name == "then") {
            return parse_then_pattern(function_name, r_expression);
        }

        else if (function_name == "with") {
            return parse_with_pattern(function_name, r_expression);
        }

        else if (function_name == "&&" || function_name == "all") {
            return create_variadic_pattern<AllVariadicPattern>(function_name,
                                                               r_expression);
        }

        else if (function_name == "||" || function_name == "any") {
            return create_variadic_pattern<AnyVariadicPattern>(function_name,
                                                               r_expression);
        }

        else if (function_name == "!" || function_name == "none") {
            return create_variadic_pattern<NoneVariadicPattern>(function_name,
                                                                r_expression);
        }

        else if (function_name == "vector" || function_name == "vec") {
            return parse_greedy_pattern_sequence<VectorUnaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "integer" || function_name == "int") {
            return parse_greedy_pattern_sequence<IntegerUnaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "double" || function_name == "dbl" ||
                 function_name == "real") {
            return parse_greedy_pattern_sequence<RealUnaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "logical" || function_name == "lgl") {
            return parse_greedy_pattern_sequence<LogicalUnaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "raw") {
            return parse_greedy_pattern_sequence<RawUnaryPattern>(function_name,
                                                                  r_expression);
        }

        else if (function_name == "character" || function_name == "chr") {
            return parse_greedy_pattern_sequence<CharacterUnaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "list") {
            return parse_greedy_pattern_sequence<ListUnaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "pairlist" || function_name == "pair") {
            return parse_greedy_pattern_sequence<PairlistUnaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "closure") {
            return parse_ternary_pattern_sequence<ClosureTernaryPattern>(
                function_name, r_expression);
        }

        else if (function_name == "language" || function_name == "lang") {
            return parse_greedy_pattern_sequence<LanguageUnaryPattern>(
                function_name, r_expression);
        }
    }

    std::string message = "invalid pattern";
    return Result(message);
}
