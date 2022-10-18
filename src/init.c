#include <R_ext/Rdynload.h>
#include <stdio.h>
#include "matchr.h"

static const R_CallMethodDef CallEntries[] = {
    {"matchr_parse", (DL_FUNC) &r_matchr_parse, 3},
    {"matchr_match", (DL_FUNC) &r_matchr_match, 2},
    {"matchr_expr", (DL_FUNC) &r_matchr_expr, 1},
    {NULL, NULL, 0}
};

void R_init_matchr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

