
#' @export
match_file <- function(object, ...) {
    UseMethod("match_file")
}

#' @export
match_file.matchr_matcher <- function(object, ...) {
    filename <- list(...)[[1]]

    stopifnot(is_scalar_character(filename))

    do.call(match_values, c(object, parse_file(filename)))
}

#' @export
match_file.matchr_clause <- function(object, ...) {
    clauses <- c(object, list(...))

    size <- length(clauses)

    filename <- clauses[[size]]

    clauses[[size]] <- NULL

    m <- do.call(matcher, clauses)

    match_file(m, filename)
}
