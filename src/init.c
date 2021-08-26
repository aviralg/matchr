#include <R_ext/Rdynload.h>
#include <stdio.h>
#include "matchr.h"

static const R_CallMethodDef CallEntries[] = {
    {"matchr_initialize", (DL_FUNC) &matchr_initialize, 1},
    {"matchr_finalize", (DL_FUNC) &matchr_finalize, 0},
    {"matchr_create", (DL_FUNC) &matchr_create, 3},
    {"matchr_match", (DL_FUNC) &matchr_match, 2},
    {NULL, NULL, 0}
};

void R_init_matchr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

