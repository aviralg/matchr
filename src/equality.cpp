#include "equality.h"
#include <R_ext/Complex.h>
#include <cstring>

bool equal_symbols(SEXP symbol1, SEXP symbol2) {
    return !strcmp(CHAR(PRINTNAME(symbol1)), CHAR(PRINTNAME(symbol2)));
}

bool equal_languages(SEXP lang1, SEXP lang2) {
    int len1 = LENGTH(lang1);
    int len2 = LENGTH(lang2);

    if (len1 != len2) {
        return false;
    }

    SEXP ptr1 = lang1;
    SEXP ptr2 = lang2;

    while (ptr1 != R_NilValue) {
        if (!is_equal(CAR(ptr1), CAR(ptr2))) {
            return false;
        }
        ptr1 = CDR(ptr1);
        ptr2 = CDR(ptr2);
    }

    return true;
}

bool equal_lists(SEXP list1, SEXP list2) {
    int len1 = LENGTH(list1);
    int len2 = LENGTH(list2);

    if (len1 != len2) {
        return false;
    }

    SEXP ptr1 = list1;
    SEXP ptr2 = list2;

    while (ptr1 != R_NilValue) {
        if (!is_equal(CAR(ptr1), CAR(ptr2))) {
            return false;
        }
        ptr1 = CDR(ptr1);
        ptr2 = CDR(ptr2);
    }

    return true;
}

bool equal_logical_vectors(SEXP logical1, SEXP logical2) {
    int len1 = LENGTH(logical1);
    int len2 = LENGTH(logical2);

    if (len1 != len2) {
        return false;
    }

    for (int index = 0; index < len1; ++index) {
        int value1 = LOGICAL(logical1)[index];
        int value2 = LOGICAL(logical2)[index];
        if (value1 != value2) {
            return false;
        }
    }

    return true;
}

bool equal_integer_vectors(SEXP int1, SEXP int2) {
    int len1 = LENGTH(int1);
    int len2 = LENGTH(int2);

    if (len1 != len2) {
        return false;
    }

    for (int index = 0; index < len1; ++index) {
        int value1 = INTEGER(int1)[index];
        int value2 = INTEGER(int2)[index];
        if (value1 != value2) {
            return false;
        }
    }

    return true;
}

bool equal_double_vectors(SEXP dbl1, SEXP dbl2) {
    int len1 = LENGTH(dbl1);
    int len2 = LENGTH(dbl2);

    if (len1 != len2) {
        return false;
    }

    for (int index = 0; index < len1; ++index) {
        double value1 = REAL(dbl1)[index];
        double value2 = REAL(dbl2)[index];
        if (value1 != value2) {
            return false;
        }
    }

    return true;
}

bool equal_complex_vectors(SEXP cplx1, SEXP cplx2) {
    int len1 = LENGTH(cplx1);
    int len2 = LENGTH(cplx2);

    if (len1 != len2) {
        return false;
    }

    for (int index = 0; index < len1; ++index) {
        Rcomplex value1 = COMPLEX_ELT(cplx1, index);
        Rcomplex value2 = COMPLEX_ELT(cplx2, index);

        if (value1.r != value2.r || value1.i != value2.i) {
            return false;
        }
    }

    return true;
}

bool equal_string_vectors(SEXP str1, SEXP str2) {
    int len1 = LENGTH(str1);
    int len2 = LENGTH(str2);

    if (len1 != len2) {
        return false;
    }

    for (int index = 0; index < len1; ++index) {
        const char* value1 = CHAR(STRING_ELT(str1, index));
        const char* value2 = CHAR(STRING_ELT(str2, index));
        if (strcmp(value1, value2)) {
            return false;
        }
    }

    return true;
}

bool is_equal(SEXP value1, SEXP value2) {
    if (TYPEOF(value1) != TYPEOF(value2)) {
        return false;
    }

    switch (TYPEOF(value1)) {
    case NILSXP:
        /* return true if both are R_NilValue  */
        return true;
        break;

    case SYMSXP:
        return equal_symbols(value1, value2);
        break;

    case LISTSXP:
        return equal_lists(value1, value2);
        break;

    case CLOSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case ENVSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case PROMSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case LANGSXP:
        /* TODO: cannot happen, raise error */
        return equal_languages(value1, value2);
        break;

    case SPECIALSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case BUILTINSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case CHARSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case LGLSXP:
        /* TODO: cannot happen, raise error */
        return equal_logical_vectors(value1, value2);
        break;

    case INTSXP:
        /* TODO: cannot happen, raise error */
        return equal_integer_vectors(value1, value2);
        break;

    case REALSXP:
        /* TODO: cannot happen, raise error */
        return equal_double_vectors(value1, value2);
        break;

    case CPLXSXP:
        /* TODO: cannot happen, raise error */
        return equal_complex_vectors(value1, value2);
        break;

    case STRSXP:
        /* TODO: cannot happen, raise error */
        return equal_string_vectors(value1, value2);
        break;

    case DOTSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case ANYSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case VECSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case EXPRSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case BCODESXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case EXTPTRSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case WEAKREFSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case RAWSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case S4SXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case NEWSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case FREESXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;

    case FUNSXP:
        /* TODO: cannot happen, raise error */
        return false;
        break;
    }

    /* TODO: should not reach here, raise error */
    return false;
}
