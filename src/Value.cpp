#include "Value.h"

Value* Value::take(int size) const {
    return new SliceValue(
        get_value(), this->tf_index(0), this->tf_index(size - 1));
}

Value* Value::drop(int size) const {
    return new SliceValue(get_value(),
                          this->tf_index(size),
                          this->tf_index(this->get_size() - 1));
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
            if (this->get_dbl(i) != other->get_dbl(i)) {
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
        SEXP r_vec = Rf_allocVector(LGLSXP, size);
        for (int i = 0; i < size; ++i) {
            LOGICAL(r_vec)[i] = this->get_lgl(i);
        }
        return r_vec;
    }

    case INTSXP: {
        int size = this->get_size();
        SEXP r_vec = Rf_allocVector(INTSXP, size);
        for (int i = 0; i < size; ++i) {
            INTEGER(r_vec)[i] = this->get_int(i);
        }
        return r_vec;
    }

    case REALSXP: {
        int size = this->get_size();
        SEXP r_vec = Rf_allocVector(REALSXP, size);
        for (int i = 0; i < size; ++i) {
            REAL(r_vec)[i] = this->get_dbl(i);
        }
        return r_vec;
    }

    default:
        Rf_error("Unhandled SEXP type in Value::to_sexp");
        return R_NilValue;
    }
}
