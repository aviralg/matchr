#include "utils.h"

SEXP deparse(SEXP r_val, bool quote) {
    if (quote) {
        r_val = Rf_lang2(Rf_install("quote"), r_val);
    }

    PROTECT(r_val);

    SEXP r_call = PROTECT(lang2(Rf_install("deparse"), r_val));

    SEXP r_res = PROTECT(Rf_eval(r_call, R_BaseEnv));

    UNPROTECT(3);

    return r_res;
}
