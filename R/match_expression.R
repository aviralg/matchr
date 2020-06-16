
#' @export
match_expression <- function(object, ...) {
    UseMethod("match_expression")
}

#' @export
match_expression.matchr_matcher <- function(object, ...) { # nolint
    exprs <- list(...)
    lapply(exprs, function(expr) .Call(C_matcher_match_expression, object, expr))
}
