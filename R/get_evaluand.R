
#' @export
get_evaluand <- function(object, ...) {
    UseMethod("get_evaluand")
}

#' @export
get_evaluand.matchr_clause <- function(object, ...) {
    .Call(C_clause_get_evaluand, object)
}
