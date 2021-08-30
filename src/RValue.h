#ifndef MATCHR_R_VALUE_H
#define MATCHR_R_VALUE_H

#include <string>
#include "r.h"

class RValue {
  public:
    /* TODO: check if all R values have length or not */
    RValue(SEXP r_value)
        : r_value_(r_value), begin_(0), end_(Rf_length(r_value)) {
    }

    RValue(SEXP r_value, int begin, int end)
        : r_value_(r_value), begin_(begin), end_(end) {
    }

    SEXP get_value() const {
        return r_value_;
    }

    SEXPTYPE get_type() const {
        return TYPEOF(get_value());
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

    bool is_character_scalar() {
        return is_character_vector() && get_length() == 1;
    }

    std::string get_character_element(int index) {
        int new_index = transform_index_(index);
        return CHAR(STRING_ELT(get_value(), new_index));
    }

    /***************************************************************************
     * LOGICAL
     **************************************************************************/

    bool is_logical_vector() const {
        return has_type(LGLSXP);
    }

    bool is_logical_scalar() {
        return is_logical_vector() && get_length() == 1;
    }

    int get_logical_element(int index) {
        int new_index = transform_index_(index);
        return LOGICAL(get_value())[new_index];
    }

    /***************************************************************************
     * REAL
     **************************************************************************/

    bool is_real_vector() const {
        return has_type(REALSXP);
    }

    bool is_real_scalar() {
        return is_real_vector() && get_length() == 1;
    }

    double get_real_element(int index) {
        int new_index = transform_index_(index);
        return REAL(get_value())[new_index];
    }

    /***************************************************************************
     * INTEGER
     **************************************************************************/

    bool is_integer_vector() const {
        return has_type(INTSXP);
    }

    bool is_integer_scalar() {
        return is_integer_vector() && get_length() == 1;
    }

    int get_integer_element(int index) {
        int new_index = transform_index_(index);
        return INTEGER(get_value())[new_index];
    }

    /***************************************************************************
     * COMPLEX
     **************************************************************************/

    bool is_complex_vector() const {
        return has_type(CPLXSXP);
    }

    bool is_complex_scalar() {
        return is_complex_vector() && get_length() == 1;
    }

    const Rcomplex& get_complex_element(int index) {
        int new_index = transform_index_(index);
        return COMPLEX(get_value())[new_index];
    }

    /***************************************************************************
     * RAW
     **************************************************************************/

    bool is_raw_vector() const {
        return has_type(RAWSXP);
    }

    bool is_raw_scalar() {
        return is_raw_vector() && get_length() == 1;
    }

    Rbyte get_raw_element(int index) {
        int new_index = transform_index_(index);
        return RAW_ELT(get_value(), new_index);
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

        return RValue(r_value_, new_begin, new_end);
    }

  private:
    SEXP r_value_;
    int begin_;
    int end_;

    int transform_index_(int index) {
        int new_index = begin_ + index;
        if (new_index >= end_) {
            Rf_error("out of bounds access");
        }
        return new_index;
    }
};

#endif /* MATCHR_R_VALUE_H */
