#' @useDynLib matchr, .registration = TRUE, .fixes = "C_"
NULL


#' @export
match <- function(value,
                  ...,
                  pat_env = parent.frame(),
                  eval_env = parent.frame()) {
    object <- matcher(..., pat_env = pat_env, eval_env = eval_env)
    object(value)
}


#' @export
matcher <- function(...,
                    pat_env = parent.frame(),
                    eval_env = parent.frame()) {

    clauses <- list(..., . ~ stop("value did not match any pattern"))

    object <- .Call(C_matchr_create, clauses, pat_env, eval_env)

    m <- function(value) {
        .Call(C_matchr_match, object, value)
    }

    structure(m, class = "matchr_matcher")
}

#' @export
print.matchr_matcher <- function(x, ...) {
    ## TODO
}
