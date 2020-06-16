
#' @export
clause <- function(pattern, evaluand) {
    is_pattern(pattern)
    is_evaluand(evaluand)

    .Call(C_clause_create_clause, pattern, evaluand)
}

#' @export
`%~>%` <- clause # nolint
