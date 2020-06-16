
#' @export
get_expression <- function(object, ...) {
    UseMethod("get_expression")
}

#' @export
get_expression.matchr_pattern <- function(object, ...) {
    .Call(C_pattern_get_expression, object)
}

#' @export
get_expression.matchr_evaluand <- function(object, ...) {
    .Call(C_evaluand_get_expression, object)
}
