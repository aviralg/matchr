#include "Value.h"

Value* Value::take(int size) const {
    return new SliceValue(this, 0, size - 1);
}

Value* Value::drop(int size) const {
    return new SliceValue(this, size, this->get_size() - 1);
}

Value* Value::pick(int index) const {
    return new SliceValue(this, index, index);
}

bool Value::is_equal(Value* other) const {
    if (this->get_type() != other->get_type()) {
        return false;
    }

    switch (this->get_type()) {
    case NILSXP:
        return true;

    case LGLSXP: {
        int size = this->get_size();
        if (size != other->get_size()) {
            return false;
        }

        for (int i = 0; i < size; ++i) {
            if (this->get_lgl(i) != other->get_lgl(i)) {
                return false;
            }
        }

        return true;
    }

    case INTSXP: {
        int size = this->get_size();
        if (size != other->get_size()) {
            return false;
        }

        for (int i = 0; i < size; ++i) {
            if (this->get_int(i) != other->get_int(i)) {
                return false;
            }
        }

        return true;
    }

    case REALSXP: {
        int size = this->get_size();
        if (size != other->get_size()) {
            return false;
        }

        for (int i = 0; i < size; ++i) {
            if (!cmp_dbl(this->get_dbl(i), other->get_dbl(i))) {
                return false;
            }
        }

        return true;
    }

    case STRSXP: {
        int size = this->get_size();
        if (size != other->get_size()) {
            return false;
        }

        for (int i = 0; i < size; ++i) {
            if (!cmp_str(this->get_str(i), other->get_str(i))) {
                return false;
            }
        }

        return true;
    }

    case CPLXSXP: {
        int size = this->get_size();
        if (size != other->get_size()) {
            return false;
        }

        for (int i = 0; i < size; ++i) {
            if (!cmp_cpx(this->get_cpx(i), other->get_cpx(i))) {
                return false;
            }
        }

        return true;
    }

    case RAWSXP: {
        int size = this->get_size();
        if (size != other->get_size()) {
            return false;
        }

        for (int i = 0; i < size; ++i) {
            if (this->get_raw(i) != other->get_raw(i)) {
                return false;
            }
        }

        return true;
    }

    case VECSXP: {
        int size = this->get_size();
        if (size != other->get_size()) {
            return false;
        }

        for (int i = 0; i < size; ++i) {
            Value* elt1 = this->get_elt(i);
            Value* elt2 = other->get_elt(i);
            if (!elt1->is_equal(elt2)) {
                return false;
            }
        }

        return true;
    }

    default:
        return false;
    }
}

SEXP Value::to_sexp() const {
    switch (this->get_type()) {
    case NILSXP:
        return R_NilValue;

    case LGLSXP: {
        int size = this->get_size();
        SEXP r_vec = PROTECT(Rf_allocVector(LGLSXP, size));
        for (int i = 0; i < size; ++i) {
            LOGICAL(r_vec)[i] = this->get_lgl(i);
        }
        UNPROTECT(1);
        return r_vec;
    }

    case INTSXP: {
        int size = this->get_size();
        SEXP r_vec = PROTECT(Rf_allocVector(INTSXP, size));
        for (int i = 0; i < size; ++i) {
            INTEGER(r_vec)[i] = this->get_int(i);
        }
        UNPROTECT(1);
        return r_vec;
    }

    case REALSXP: {
        int size = this->get_size();
        SEXP r_vec = PROTECT(Rf_allocVector(REALSXP, size));
        for (int i = 0; i < size; ++i) {
            REAL(r_vec)[i] = this->get_dbl(i);
        }
        UNPROTECT(1);
        return r_vec;
    }

    case STRSXP: {
        int size = this->get_size();
        SEXP r_vec = PROTECT(Rf_allocVector(STRSXP, size));
        for (int i = 0; i < size; ++i) {
            SET_STRING_ELT(r_vec, i, str_wrap(this->get_str(i)));
        }
        UNPROTECT(1);
        return r_vec;
    }

    case CPLXSXP: {
        int size = this->get_size();
        SEXP r_vec = PROTECT(Rf_allocVector(CPLXSXP, size));
        for (int i = 0; i < size; ++i) {
            SET_COMPLEX_ELT(r_vec, i, this->get_cpx(i));
        }
        UNPROTECT(1);
        return r_vec;
    }

    case RAWSXP: {
        int size = this->get_size();
        SEXP r_vec = PROTECT(Rf_allocVector(RAWSXP, size));
        for (int i = 0; i < size; ++i) {
            RAW(r_vec)[i] = this->get_raw(i);
        }
        UNPROTECT(1);
        return r_vec;
    }

    case VECSXP: {
        int size = this->get_size();
        SEXP r_vec = PROTECT(Rf_allocVector(VECSXP, size));
        for (int i = 0; i < size; ++i) {
            SEXP r_elt = PROTECT(this->get_elt(i)->to_sexp());
            SET_VECTOR_ELT(r_vec, i, r_elt);
            UNPROTECT(1);
        }
        UNPROTECT(1);
        return r_vec;
    }

    default:
        Rf_error("Unhandled SEXP type in Value::to_sexp");
        return R_NilValue;
    }
}
