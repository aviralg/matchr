
#' @export
pattern <- function(expression, environment = parent.frame(), quote = TRUE) {

    if (quote) expression <- substitute(expression)

    stopifnot(is_environment(environment))

    create_pattern(expression, environment)
}

range <- function(expression, minimum = 0L, maximum = 2147483647L) {
    expression <- substitute(expression)

    stopifnot(is_language(expression))
    stopifnot(is_scalar_integer(minimum) || is_scalar_real(minimum))
    stopifnot(is_scalar_integer(maximum) || is_scalar_real(maximum))
    stopifnot(minimum < maximum)

    minimum <- as.integer(minimum)
    maximum <- as.integer(maximum)

    list(expression = expression, minimum = minimum, maximum = maximum)
}

create_pattern <- function(expression, environment) {

    if (is_symbol(expression)) {

        value <- as.character(expression)

        if (value == ".") {
            .Call(C_pattern_create_wildcard_pattern, expression, environment, value)
        }

        else {
            .Call(C_pattern_create_identifier_pattern, expression, environment, value)
        }

    }

    else if (is_scalar_integer(expression)) {
        .Call(C_pattern_create_integer_literal_pattern, expression, environment, expression)
    }

    else if (is_scalar_real(expression)) {
        .Call(C_pattern_create_real_literal_pattern, expression, environment, expression)
    }

    else if (is_scalar_character(expression)) {
        .Call(C_pattern_create_string_literal_pattern, expression, environment, expression)
    }

    else if (is_scalar_logical(expression)) {
        .Call(C_pattern_create_logical_literal_pattern, expression, environment, expression)
    }

    else if (is_language(expression)) {

        function_name <- as.character(expression[[1]])

        argument_count <- length(expression) - 1

        if (function_name == "raw" && argument_count == 1 && is_scalar_raw_coercible(expression[[2]])) {
            .Call(C_pattern_create_raw_literal_pattern, expression, environment, as.raw(expression[[2]]))
        }

        else if (function_name == "+" && argument_count == 2 && is_numeric(expression[[2]]) && is_scalar_complex(expression[[3]])) {
            value <- expression[[2]] + expression[[3]]
            .Call(C_pattern_create_complex_literal_pattern, expression, environment, value)
        }

        ## NULLARY PATTERNS

        else if (function_name == "predicate" && argument_count == 1) {
            predicate <- expression[[2]]

            .Call(C_pattern_create_predicate_pattern, expression, environment, predicate)
        }

        ## UNARY PATTERNS

        else if (function_name == "!" && argument_count == 1) {
            sub_pattern <- create_pattern(expression[[2]], environment)

            .Call(C_pattern_create_not_unary_pattern, expression, environment, sub_pattern)
        }

        else if (function_name == "(" && argument_count == 1) {
            sub_pattern <- create_pattern(expression[[2]], environment)

            .Call(C_pattern_create_group_unary_pattern, expression, environment, sub_pattern)
        }

        else if (function_name == "range") {
            result <- eval(expression)

            sub_pattern <- create_pattern(result$expression, environment)

            .Call(C_pattern_create_range_unary_pattern, expression, environment, sub_pattern, result$minimum, result$maximum)
        }

        ## BINARY PATTERNS

        else if (function_name == "&&" && argument_count == 2) {
            left_sub_pattern <- create_pattern(expression[[2]], environment)

            right_sub_pattern <- create_pattern(expression[[3]], environment)

            .Call(C_pattern_create_and_binary_pattern, expression, environment, left_sub_pattern, right_sub_pattern)
        }

        else if (function_name == "||" && argument_count == 2) {
            left_sub_pattern <- create_pattern(expression[[2]], environment)

            right_sub_pattern <- create_pattern(expression[[3]], environment)

            left_identifier_names <- .Call(C_pattern_get_identifier_names, left_sub_pattern)

            right_identifier_names <- .Call(C_pattern_get_identifier_names, right_sub_pattern)

            if(length(setdiff(left_identifier_names, right_identifier_names)) != 0) {
                message <- sprintf("sub patterns '%s' and '%s' of || should introduce same bindings",
                                   expr_to_string(expression[[2]]),
                                   expr_to_string(expression[[3]]))
                stop(message)
            }

            .Call(C_pattern_create_or_binary_pattern, expression, environment, left_sub_pattern, right_sub_pattern)
        }

        ## VARIADIC PATTERNS

        else if (function_name == "vector") {
            patterns <- list()

            if(argument_count >= 1) {
                patterns <- lapply(expression[2:length(expression)],
                                   function(e) create_pattern(e, environment))
            }

            .Call(C_pattern_create_vector_variadic_pattern, expression, environment, patterns)
        }

        else {
            stop(sprintf("%s is not a valid pattern", expr_to_string(expression)))
        }
    }

    else {
        stop(sprintf("%s is not a valid pattern", expr_to_string(expression)))
    }
}
