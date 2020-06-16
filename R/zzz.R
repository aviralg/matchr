
.onLoad <- function(libname, pkgname) { # nolint

    .Call(C_matchr_initialize_matchr)

}

.onUnload <- function(libpath) { # nolint

    .Call(C_matchr_finalize_matchr)

}

.onDetach <- function(libpath) { # nolint

    .Call(C_matchr_finalize_matchr)

}
