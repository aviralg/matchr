
.onLoad <- function(libname, pkgname) { # nolint

    .Call(C_matchr_initialize, getNamespace(pkgname))

}

.onUnload <- function(libpath) { # nolint

    .Call(C_matchr_finalize)

}

.onDetach <- function(libpath) { # nolint

    .Call(C_matchr_finalize)

}
