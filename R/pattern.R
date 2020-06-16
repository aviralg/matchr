
#' @export
pattern <- function(expression, quote = TRUE) {

    if (quote) expression <- substitute(expression)

    is_language(expression)

    .Call(C_pattern_create_pattern, expression)
}
