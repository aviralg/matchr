#include "r_matchr.h"
#include "matchr.h"

SEXP r_matchr_initialize_matchr(SEXP r_package_environment) {
    matchr::initialize_matchr(r_package_environment);
    return R_NilValue;
}
SEXP r_matchr_finalize_matchr() {
    matchr::finalize_matchr();
    return R_NilValue;
}
