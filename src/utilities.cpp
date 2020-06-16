#include "utilities.h"

SEXP NewDotEnvSymbol = NULL;
SEXP PackageEnvironment = NULL;

void initialize_utilities(SEXP r_package_environment) {
    NewDotEnvSymbol = Rf_install("new.env");

    PackageEnvironment = r_package_environment;
}

SEXP new_dot_env(SEXP hash, SEXP parent, SEXP size) {
    return Rf_eval(Rf_lang4(NewDotEnvSymbol, hash, parent, size), PackageEnvironment);
}
