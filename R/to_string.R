
to_string <- function(object, ...) {
    UseMethod("to_string")
}

#' @export
to_string.matchr_matcher <- function(object, ...) { # nolint

    clauses <- vapply(get_clauses(object),
                      to_string,
                      "character")

    representation <- sprintf("matcher(%s)", paste(clauses, collapse = ",\n"))

    representation
}

#' @export
to_string.matchr_clause <- function(object, ...) {
    pat <- get_pattern(object)

    evl <- get_evaluand(object)

    representation <- sprintf("%s %%~>%% %s", to_string(pat), to_string(evl))

    representation
}

#' @export
to_string.matchr_pattern <- function(object, ...) {
    expr <- get_expression(object)

    representation <- sprintf("pattern(%s)", expr_to_string(expr))

    representation
}

#' @export
to_string.matchr_evaluand <- function(object, ...) {
    expr <- get_expression(object)

    env <- get_environment(object)

    representation <- sprintf("evaluand(%s, %s)",
                              expr_to_string(expr),
                              "<environment>")

    representation
}

expr_to_string <- function(expr) {
    paste(deparse(expr), collapse = "\n")
}
