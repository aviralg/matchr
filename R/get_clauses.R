
#' @export
get_clauses <- function(object, ...) {
    UseMethod("get_clauses")
}

#' @export
get_clauses.matchr_matcher <- function(object, ...) {
    .Call(C_matcher_get_clauses, object)
}
