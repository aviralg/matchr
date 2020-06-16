
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

