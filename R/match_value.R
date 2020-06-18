
#' @export
match_value <- function(object, ...) {
    UseMethod("match_value")
}

#' @export
match_value.matchr_matcher <- function(object, ...) { # nolint
    value <- list(...)[[1]]

    .Call(C_matcher_match_value, object, value)
}

#' @export
match_value.matchr_clause <- function(object, ...) { # nolint
    clauses <- c(object, list(...))

    size <- length(clauses)

    value <- clauses[[size]]

    clauses[[size]] <- NULL

    m <- do.call(matcher, clauses)

    match_value(m, value)
}
