
#' @useDynLib matchr, .registration = TRUE, .fixes = "C_"
NULL


#' @export
matchv <- function(value,
                  ...,
                  pat_env = parent.frame(),
                  eval_env = parent.frame()) {
    f <- matcher(..., pat_env = pat_env, eval_env = eval_env)
    f(value)
}

#' @export
matchq <- function(value,
                   ...,
                   pat_env = parent.frame(),
                   eval_env = parent.frame()) {
    value <- substitute(value)
    f <- matcher(..., pat_env = pat_env, eval_env = eval_env)
    f(value)
}

#' @export
matcher <- function(...,
                    pat_env = parent.frame(),
                    eval_env = parent.frame()) {

    clauses <- list(..., . ~ stop("value did not match any pattern"))

    object <- .Call(C_matchr_parse, clauses, pat_env, eval_env)

    m <- function(value) {
        .Call(C_matchr_match, object, value)
    }

    structure(m, class = "matchr_matcher", matcher = object)
}

#' @export
print.matchr_matcher <- function(x, ...) {
    object <- attr(x, "matcher")
    expr <- .Call(C_matchr_expr, object)
    print(expr)
    x
}
