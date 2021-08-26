
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

create_pattern <- function(expression, environment, parents) {

    add_parent <- function(parent) {
        parents <<- c(parents, parent)
    }

    wrap_literal <- function(literal_pattern) {
        if ("vector" %in% parents) {
            add_parent("literal")
            literal_pattern

        }
        else {
            add_parent("vector")
            .Call(C_pattern_create_vector_variadic_pattern, expression, environment, list(literal_pattern))
        }
    }

    if (is_symbol(expression)) {

        parsed_form <- .Call(C_utilities_parse_identifier, as.character(expression))

        type <- parsed_form$type
        identifier <- parsed_form$identifier
        minimum <- parsed_form$minimum
        maximum <- parsed_form$maximum

        if (type == "identifier") {
            if(identifier == ".") {
                .Call(C_pattern_create_wildcard_pattern, expression, environment)
            }
            else {
                .Call(C_pattern_create_identifier_pattern, expression, environment, identifier)
            }
        }
        else {
            add_parent("range")

            sub_pattern <- create_pattern(identifier, environment, parents)

            minimum <- as.integer(minimum)

            if (is.na(minimum)) {
                messsage <- sprintf("minimum of range pattern, '%s' is not an integer", minimum)
                stop(message)
            }

            maximum <- as.integer(maximum)

            if (is.na(maximum)) {
                messsage <- sprintf("maximum of range pattern, '%s' is not an integer", maximum)
                stop(message)
            }

            .Call(C_pattern_create_range_unary_pattern, expression, environment, sub_pattern, minimum, maximum)
        }
    }

    else if (is_scalar_integer(expression)) {
        wrap_literal(.Call(C_pattern_create_integer_literal_pattern, expression, environment, expression))
    }

    else if (is_scalar_real(expression)) {
        wrap_literal(.Call(C_pattern_create_real_literal_pattern, expression, environment, expression))
    }

    else if (is_scalar_character(expression)) {
        wrap_literal(.Call(C_pattern_create_string_literal_pattern, expression, environment, expression))
    }

    else if (is_scalar_logical(expression)) {
        wrap_literal(.Call(C_pattern_create_logical_literal_pattern, expression, environment, expression))
    }

    else if (is_language(expression)) {

        function_name <- as.character(expression[[1]])

        argument_count <- length(expression) - 1

        if (function_name == "raw" && argument_count == 1 && is_scalar_raw_coercible(expression[[2]])) {
            wrap_literal(.Call(C_pattern_create_raw_literal_pattern, expression, environment, as.raw(expression[[2]])))
        }

        else if (function_name == "+" && argument_count == 2 && is_numeric(expression[[2]]) && is_scalar_complex(expression[[3]])) {
            value <- expression[[2]] + expression[[3]]
            wrap_literal(.Call(C_pattern_create_complex_literal_pattern, expression, environment, value))
        }

        ## NULLARY PATTERNS

        else if (function_name == "predicate" && argument_count == 1) {
            add_parent("predicate")

            predicate <- expression[[2]]

            .Call(C_pattern_create_predicate_pattern, expression, environment, predicate)
        }

        ## UNARY PATTERNS

        else if (function_name == "!" && argument_count == 1) {
            add_parent("!")

            sub_pattern <- create_pattern(expression[[2]], environment, parents)

            .Call(C_pattern_create_not_unary_pattern, expression, environment, sub_pattern)
        }

        else if (function_name == "(" && argument_count == 1) {
            add_parent("(")

            sub_pattern <- create_pattern(expression[[2]], environment, parents)

            .Call(C_pattern_create_group_unary_pattern, expression, environment, sub_pattern)
        }

        else if (function_name == "range") {
            add_parent("range")

            result <- eval(expression)

            sub_pattern <- create_pattern(result$expression, environment, parents)

            .Call(C_pattern_create_range_unary_pattern, expression, environment, sub_pattern, result$minimum, result$maximum)
        }

        ## BINARY PATTERNS

        else if (function_name == "&&" && argument_count == 2) {
            add_parent("&&")

            left_sub_pattern <- create_pattern(expression[[2]], environment, parents)

            right_sub_pattern <- create_pattern(expression[[3]], environment, parents)

            .Call(C_pattern_create_and_binary_pattern, expression, environment, left_sub_pattern, right_sub_pattern)
        }

        else if (function_name == "||" && argument_count == 2) {
            add_parent("||")

            left_sub_pattern <- create_pattern(expression[[2]], environment, parents)

            right_sub_pattern <- create_pattern(expression[[3]], environment, parents)

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

        ## SEQUENCE PATTERNS

        else if (function_name == "vector") {
            if ("vector" %in% parents) {
                message <- sprintf("vector pattern '%s' cannot be nested inside another vector pattern", expr_to_string(expression))
                stop(message)
            }

            add_parent("vector")

            patterns <- list()

            if (argument_count >= 1) {
                patterns <- lapply(expression[2:length(expression)],
                                   function(e) create_pattern(e, environment, parents))
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
