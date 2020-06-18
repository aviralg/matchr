
#' @export
match_files <- function(object, ...) {
    UseMethod("match_files")
}

#' @export
match_files.matchr_matcher <- function(object, ...) {
    filenames <- unlist(list(...))

    stopifnot(is_character(filenames))

    ## create list of lists of language objects
    contents <- lapply(filenames, parse_file)

    ## concatenate to a list of language objects
    contents <- do.call(c, contents)

    do.call(match_values, c(object, contents))
}

#' @export
match_files.matchr_clause <- function(object, ...) {

    clauses <- c(object, list(...))

    count <- length(clauses)
    index <- 1

    while (index <= count) {
        if (is_clause(clauses[[index]])) {
            index <- index + 1
        }
        else {
            break
        }
    }

    if (index > count) {
        stop("no files provided for matching")
    }

    filenames <- clauses[index:count]

    clauses <- clauses[1:(index - 1)]

    m <- do.call(matcher, clauses)

    do.call(match_files, c(m, filenames))
}
