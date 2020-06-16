
#' @export
get_pattern <- function(object, ...) {
    UseMethod("get_pattern")
}

#' @export
get_pattern.matchr_clause <- function(object, ...) {
    .Call(C_clause_get_pattern, object)
}
