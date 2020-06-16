
#' @export
evaluand <- function(expression, environment = parent.frame(), quote = TRUE) {
    if (quote) expression <- substitute(expression)

    is_language(expression)
    is_environment(environment)

    .Call(C_evaluand_create_evaluand, expression, environment)
}
