#include "Pattern.h"
#include "AllVariadicPattern.h"
#include "ComplexLiteralPattern.h"
#include "GroupUnaryPattern.h"
#include "IntegerLiteralPattern.h"
#include "LogicalLiteralPattern.h"
#include "NoneVariadicPattern.h"
#include "AnyVariadicPattern.h"
#include "PredicatePattern.h"
#include "RangeUnaryPattern.h"
#include "RawLiteralPattern.h"
#include "RealLiteralPattern.h"
#include "CharacterLiteralPattern.h"
#include "VectorUnaryPattern.h"
#include "WildcardPattern.h"
#include "SatisfiesPattern.h"
#include "SequencePattern.h"
#include "WhenUnaryPattern.h"

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
Result create_unary_pattern(const std::string& function_name,
                            SEXP r_expression) {
    Result inner = create_helper(CADR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return Result(new T(r_expression, inner.get_pattern()));
}

Result create_when_pattern(const std::string& function_name,
                           SEXP r_expression) {
    if (Rf_length(r_expression) != 3) {
        return Result("incorrect number of arguments passed to when");
    }

    Result inner = create_helper(CADR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    return Result(new WhenUnaryPattern(
        r_expression, inner.get_pattern(), CADDR(r_expression)));
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

        else if (function_name == "?" || function_name == "satisfies") {
            return Result(
                new SatisfiesPattern(r_expression, CADR(r_expression)));

        }

        else if (function_name == "when") {
            return create_when_pattern(function_name, r_expression);
        }

        else if (function_name == "range") {
            return create_range_pattern(function_name, r_expression);
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
            return create_sequence_pattern<VectorUnaryPattern>(function_name,
                                                               r_expression);
        }
    }

    std::string message = "invalid pattern";
    return Result(message);
}
