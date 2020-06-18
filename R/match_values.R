
#' @export
match_values <- function(object, ...) {
    UseMethod("match_values")
}

#' @export
match_values.matchr_matcher <- function(object, ...) { # nolint
    values <- list(...)

    lapply(values, function(value) match_value(object, value))
}

#' @export
match_values.matchr_clause <- function(object, ...) { # nolint
    clauses <- c(object, list(...))

    count <- length(clauses)
    index <- 1

    while (index <= count) {
        if (is_clause(clauses[[index]])) {
            index <- index + 1
        }
        else {
            break
        }
    }

    if (index > count) {
        stop("no values provided for matching")
    }

    values <- clauses[index:count]
    clauses <- clauses[1:(index - 1)]

    m <- do.call(matcher, clauses)

    do.call(match_values, c(m, values))
}
