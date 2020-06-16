
#' @export
print.matchr_matcher <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
print.matchr_clause <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
print.matchr_pattern <- function(x, ...) {
    cat(to_string(x), "\n")
}

#' @export
print.matchr_evaluand <- function(x, ...) {
    cat(to_string(x), "\n")
}
