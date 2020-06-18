
is_matcher <- function(object) {
    inherits(object, "matchr_matcher")
}

is_clause <- function(object) {
    inherits(object, "matchr_clause")
}

is_pattern <- function(object) {
    inherits(object, "matchr_pattern")
}

is_evaluand <- function(object) {
    inherits(object, "matchr_evaluand")
}

is_language <- function(object) {
    is.language(object)
}

is_environment <- function(object) {
    is.environment(object)
}

is_scalar_character <- function(vector) {
    is.character(vector) && (length(vector) == 1) && (nchar(vector) != 0)
}

is_vector_character <- function(vector) {
    is.character(vector)
}

is_scalar_logical <- function(vector) {
    is.logical(vector) && (length(vector) == 1)
}

is_scalar_integer <- function(vector) {
    is.integer(vector) && (length(vector) == 1)
}

is_scalar_real <- function(vector) {
    is.double(vector) && (length(vector) == 1)
}

is_scalar_complex <- function(vector) {
    is.complex(vector) && (length(vector) == 1)
}

is_scalar_raw <- function(vector) {
    is.raw(vector) && (length(vector) == 1)
}

is_scalar_raw_coercible <- function(value) {
    is.numeric(value) && length(value) == 1 && value >= 0 && value <= 255
}

is_symbol <- function(object) {
    is.symbol(object)
}

is_function <- function(fun) {
    is.function(fun)
}

is_null <- function(obj) {
    is.null(obj)
}

is_closure <- function(obj) {
    typeof(obj) == "closure"
}
