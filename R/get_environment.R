
#' @export
get_environment <- function(object, ...) {
    UseMethod("get_environment")
}

#' @export
get_environment.matchr_evaluand <- function(object, ...) {
    .Call(C_evaluand_get_environment, object)
}