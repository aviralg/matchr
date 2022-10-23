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

const char* str_unwrap(SEXP char_val) {
    return (char_val == NA_STRING) ? nullptr : CHAR(char_val);
}

SEXP str_wrap(const char* str_val) {
    return (str_val == nullptr) ? NA_STRING : Rf_mkChar(str_val);
}

char* str_dup(const char* val) {
    if (val == nullptr) {
        return nullptr;
    }

    int n = std::strlen(val);
    char* dup = (char*) malloc((n + 1) * sizeof(char));
    std::strncpy(dup, val, n);
    dup[n] = '\0';
    return dup;
}

bool cmp_dbl(double val1, double val2) {
    if (ISNAN(val1))
        return ISNAN(val2);
    if (ISNAN(val2))
        return ISNAN(val1);
    return val1 == val2;
}

bool cmp_cpx(const Rcomplex& val1, const Rcomplex& val2) {
    return cmp_dbl(val1.r, val2.r) && cmp_dbl(val1.i, val2.i);
}

bool cmp_str(const char* val1, const char* val2) {
    if(val1 == nullptr) return (val2 == nullptr);
    if(val2 == nullptr) return (val1 == nullptr);
    return !strcmp(val1, val2);
}
