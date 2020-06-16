
#' @export
matcher <- function(...) {
    clauses <- list(...)

    for (clause in clauses) {
        stopifnot(is_clause(clause))
    }

    .Call(C_matcher_create_matcher, clauses)
}
