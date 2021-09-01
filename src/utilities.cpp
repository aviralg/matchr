#include "utilities.h"

SEXP NewDotEnvSymbol = NULL;
SEXP PackageEnvironment = NULL;

void initialize_utilities(SEXP r_package_environment) {
    NewDotEnvSymbol = Rf_install("new.env");

    PackageEnvironment = r_package_environment;
}

SEXP new_dot_env(bool hash, int size, SEXP r_parent) {
    SEXP r_hash = PROTECT(ScalarLogical(hash));
    SEXP r_size = PROTECT(ScalarInteger(size));

    SEXP r_env = Rf_eval(Rf_lang4(NewDotEnvSymbol, r_hash, r_parent, r_size),
                         PackageEnvironment);

    UNPROTECT(2);

    return r_env;
}
