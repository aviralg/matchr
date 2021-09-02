#ifndef MATCHR_R_VALUE_H
#define MATCHR_R_VALUE_H

#include <string>
#include "r.h"

class RValue {
  public:
    /* TODO: check if all R values have length or not */
    explicit RValue(SEXP r_value)
        : r_value_(r_value)
        , type_(TYPEOF(r_value))
        , begin_(0)
        , end_(Rf_length(r_value)) {
    }

    RValue(SEXP r_value, int begin, int end)
        : r_value_(r_value), type_(TYPEOF(r_value)), begin_(begin), end_(end) {
    }

    RValue(SEXPTYPE type, std::vector<RValue>& elements)
        : r_value_(NULL)
        , type_(type)
        , elements_(elements)
        , begin_(0)
        , end_(elements.size()) {
    }

    RValue(SEXPTYPE type, std::vector<RValue>& elements, int begin, int end)
        : r_value_(NULL)
        , type_(type)
        , elements_(elements)
        , begin_(begin)
        , end_(end) {
    }

    SEXP get_value() const {
        return r_value_;
    }

    SEXPTYPE get_type() const {
        return type_;
    }

    bool has_type(SEXPTYPE type) const {
        return get_type() == type;
    }

    int get_length() const {
        return end_ - begin_;
    }

    bool has_length(int size) const {
        return get_length() == size;
    }

    /***************************************************************************
     * VECTOR
     **************************************************************************/

    bool is_vector() const {
        SEXPTYPE type = get_type();
        return type == STRSXP || type == LGLSXP || type == REALSXP ||
               type == INTSXP || type == CPLXSXP || type == RAWSXP;
    }

    bool is_scalar() const {
        return is_vector() && has_length(1);
    }

    /***************************************************************************
     * CHARACTER
     **************************************************************************/

    bool is_character_vector() const {
        return has_type(STRSXP);
    }

    bool is_character_scalar() const {
        return is_character_vector() && get_length() == 1;
    }

    const char* get_character_element(int index) const {
        int new_index = transform_index_(index);
        if (is_abstract_()) {
            return elements_[new_index].get_character_element(0);
        }

        SEXP r_char = STRING_ELT(get_value(), new_index);

        if (r_char == NA_STRING) {
            return NULL;
        }

        return CHAR(r_char);
    }

    /***************************************************************************
     * LOGICAL
     **************************************************************************/

    bool is_logical_vector() const {
        return has_type(LGLSXP);
    }

    bool is_logical_scalar() const {
        return is_logical_vector() && get_length() == 1;
    }

    int get_logical_element(int index) const {
        int new_index = transform_index_(index);
        return is_abstract_() ? elements_[new_index].get_logical_element(0)
                              : LOGICAL(get_value())[new_index];
    }

    /***************************************************************************
     * REAL
     **************************************************************************/

    bool is_real_vector() const {
        return has_type(REALSXP);
    }

    bool is_real_scalar() const {
        return is_real_vector() && get_length() == 1;
    }

    double get_real_element(int index) const {
        int new_index = transform_index_(index);
        return is_abstract_() ? elements_[index].get_real_element(0)
                              : REAL(get_value())[new_index];
    }

    /***************************************************************************
     * INTEGER
     **************************************************************************/

    bool is_integer_vector() const {
        return has_type(INTSXP);
    }

    bool is_integer_scalar() const {
        return is_integer_vector() && get_length() == 1;
    }

    int get_integer_element(int index) const {
        int new_index = transform_index_(index);
        return is_abstract_() ? elements_[new_index].get_integer_element(0)
                              : INTEGER(get_value())[new_index];
    }

    /***************************************************************************
     * COMPLEX
     **************************************************************************/

    bool is_complex_vector() const {
        return has_type(CPLXSXP);
    }

    bool is_complex_scalar() const {
        return is_complex_vector() && get_length() == 1;
    }

    const Rcomplex& get_complex_element(int index) const {
        int new_index = transform_index_(index);
        return is_abstract_() ? elements_[new_index].get_complex_element(0)
                              : COMPLEX(get_value())[new_index];
    }

    /***************************************************************************
     * RAW
     **************************************************************************/

    bool is_raw_vector() const {
        return has_type(RAWSXP);
    }

    bool is_raw_scalar() const {
        return is_raw_vector() && get_length() == 1;
    }

    Rbyte get_raw_element(int index) const {
        int new_index = transform_index_(index);
        return is_abstract_() ? elements_[new_index].get_raw_element(0)
                              : RAW_ELT(get_value(), new_index);
    }

    /***************************************************************************
     * LIST
     **************************************************************************/

    bool is_list() const {
        return has_type(VECSXP);
    }

    /***************************************************************************
     * LANGUAGE
     **************************************************************************/

    /*
    bool is_call() const {
        return is_language();
    }

    bool is_language() const {
        return has_type(LANGSXP);
    }

    bool is_call(const std::string& name, int argument_count) const {
        if (!is_language()) {
            return false;
        }

        const RValue value(get_function_name());

        if (!value.is_symbol(name)) {
            return false;
        }

        return has_arguments(argument_count);
    }

    RValue get_function_name() const {
        return RValue(CAR(get_value()));
    }

    int get_argument_count() const {
        return get_length() - 1;
    }

    bool has_arguments(int argument_count) const {
        return get_argument_count() == argument_count;
    }
    */

    /***************************************************************************
     * SYMBOL
     **************************************************************************/

    /*
    bool is_symbol() const {
        return has_type(SYMSXP);
    }

    bool is_symbol(const std::string& name) const {
        return has_type(SYMSXP) && name == get_symbol_name();
    }

    std::string get_symbol_name() const {
        return CHAR(PRINTNAME(get_value()));
    }

    RValue get_argument(int index) {
        SEXP expr = CDR(get_value());
        for (int i = 0; i < index; ++i) {
            expr = CDR(expr);
        }
        return RValue(CAR(expr));
    }
    */

    RValue subset(int index, int length) {
        int new_begin = begin_ + index;
        int new_end = new_begin + length;

        bool correct = new_begin >= begin_ && new_begin <= end_ &&
                       new_end >= begin_ && new_end <= end_ &&
                       new_begin <= new_end;

        if (!correct) {
            Rf_error("incorrect subsetting of value[%d-%d] at index %d and "
                     "length %d",
                     begin_,
                     end_,
                     new_begin,
                     new_end);
        }

        return is_abstract_()
                   ? RValue(get_type(), elements_, new_begin, new_end)
                   : RValue(r_value_, new_begin, new_end);
    }

    SEXP to_sexp() const {
        /* if we are referring to the whole value, return as is. */
        if (r_value_ != NULL && get_length() == Rf_length(r_value_)) {
            return r_value_;
        }

        else if (is_vector()) {
            return vector_to_sexp_();
        }

        else {
            Rf_error("conversion of non-vector types to concrete values is not "
                     "yet supported");
        }
    }

    RValue extract(int index) {
        if (is_vector()) {
            return subset(index, 1);
        } else {
            /* TODO: support lists, envs, etc. */
            Rf_error("extract not supported for non vector values");
        }
    }

    bool operator==(const RValue& r_value) const {
        if (get_type() != r_value.get_type() ||
            get_length() != r_value.get_length()) {
            return false;
        }

        /* TODO: properly compare */
        return true;
    }

    bool operator!=(const RValue& r_value) const {
        return !(*this == r_value);
    }

  private:
    SEXP r_value_;
    int begin_;
    int end_;

    SEXPTYPE type_;
    std::vector<RValue> elements_;

    int transform_index_(int index) const {
        int new_index = begin_ + index;
        if (new_index >= end_) {
            Rf_error("out of bounds access");
        }
        return new_index;
    }

    SEXP vector_to_sexp_() const {
        SEXPTYPE type = get_type();
        int length = get_length();

        SEXP r_result = R_NilValue;

        if (type == INTSXP) {
            r_result = PROTECT(Rf_allocVector(INTSXP, length));

            for (int i = 0; i < length; ++i) {
                INTEGER(r_result)[i] = get_integer_element(i);
            }

            UNPROTECT(1);
        } else if (type == REALSXP) {
            r_result = PROTECT(Rf_allocVector(REALSXP, length));

            for (int i = 0; i < length; ++i) {
                REAL(r_result)[i] = get_real_element(i);
            }

            UNPROTECT(1);
        } else if (type == LGLSXP) {
            r_result = PROTECT(Rf_allocVector(LGLSXP, length));

            for (int i = 0; i < length; ++i) {
                LOGICAL(r_result)[i] = get_logical_element(i);
            }

            UNPROTECT(1);
        } else if (type == RAWSXP) {
            r_result = PROTECT(Rf_allocVector(RAWSXP, length));

            for (int i = 0; i < length; ++i) {
                RAW(r_result)[i] = get_raw_element(i);
            }

            UNPROTECT(1);
        } else if (type == CPLXSXP) {
            r_result = PROTECT(Rf_allocVector(CPLXSXP, length));

            for (int i = 0; i < length; ++i) {
                COMPLEX(r_result)[i] = get_complex_element(i);
            }

            UNPROTECT(1);
        } else {
            Rf_error("case not handled");
        }

        return r_result;
    }

    bool is_abstract_() const {
        return r_value_ == NULL;
    }
};

#endif /* MATCHR_R_VALUE_H */
