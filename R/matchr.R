
#' @useDynLib matchr, .registration = TRUE, .fixes = "C_"
NULL


#' @export
matchv <- function(value,
                  ...,
                  pat_env = parent.frame(),
                  eval_env = parent.frame(),
                  trace = FALSE) {
    f <- matcher(..., pat_env = pat_env, eval_env = eval_env)
    f(value, trace = trace)
}

#' @export
matchq <- function(value,
                   ...,
                   pat_env = parent.frame(),
                   eval_env = parent.frame(),
                   trace = FALSE) {
    value <- substitute(value)
    f <- matcher(..., pat_env = pat_env, eval_env = eval_env)
    f(value, trace = trace)
}

#' @export
matcher <- function(...,
                    pat_env = parent.frame(),
                    eval_env = parent.frame()) {

    clauses <- list(..., . ~ stop("value did not match any pattern"))

    object <- .Call(C_matchr_parse, clauses, pat_env, eval_env)

    m <- function(value, trace = FALSE) {
        .Call(C_matchr_match, object, value, trace)
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
