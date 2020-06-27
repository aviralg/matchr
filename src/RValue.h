#ifndef MATCHR_R_VALUE_H
#define MATCHR_R_VALUE_H

#include <string>
#include "Interval.h"
#include "r.h"

namespace matchr {

class RValue {
  public:
    RValue(SEXP r_value, Interval index_interval)
        : r_value_(r_value), index_interval_(index_interval) {
    }

    RValue(SEXP r_value): RValue(r_value, Interval(true, -1, -1)) {
    }

    SEXP get_value() const {
        return r_value_;
    }

    const Interval& get_index_interval() const {
        return index_interval_;
    }

    Interval& get_index_interval() {
        return index_interval_;
    }

    SEXPTYPE get_type() const {
        return TYPEOF(get_value());
    }

    bool has_type(SEXPTYPE type) const {
        return get_type() == type;
    }

    int get_length() const {
        return Rf_length(get_value());
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

    bool is_literal() const {
        return is_vector() && has_length(1);
    }

    /***************************************************************************
     * CHARACTER
     **************************************************************************/

    bool is_character_vector() const {
        return has_type(STRSXP);
    }

    bool is_character_literal() {
        return is_character_vector() && get_length() == 1;
    }

    std::string get_character_element(int index) {
        return CHAR(STRING_ELT(get_value(), index));
    }

    bool has_character_value(const std::string& value) {
        return value ==
               get_character_element(get_index_interval().get_minimum());
    }

    /***************************************************************************
     * LOGICAL
     **************************************************************************/

    bool is_logical_vector() const {
        return has_type(LGLSXP);
    }

    bool is_logical_literal() {
        return is_logical_vector() && get_length() == 1;
    }

    bool get_logical_element(int index) {
        return LOGICAL(get_value())[index];
    }

    bool has_logical_value(bool value) {
        return value == get_logical_element(get_index_interval().get_minimum());
    }

    /***************************************************************************
     * REAL
     **************************************************************************/

    bool is_real_vector() const {
        return has_type(REALSXP);
    }

    bool is_real_literal() {
        return is_real_vector() && get_length() == 1;
    }

    double get_real_element(int index) {
        return REAL(get_value())[index];
    }

    bool has_real_value(double value) {
        return value == get_real_element(get_index_interval().get_minimum());
    }

    /***************************************************************************
     * INTEGER
     **************************************************************************/

    bool is_integer_vector() const {
        return has_type(INTSXP);
    }

    bool is_integer_literal() {
        return is_integer_vector() && get_length() == 1;
    }

    long int get_integer_element(int index) {
        return INTEGER(get_value())[index];
    }

    bool has_integer_value(int value) {
        return value == get_integer_element(get_index_interval().get_minimum());
    }

    /***************************************************************************
     * COMPLEX
     **************************************************************************/

    bool is_complex_vector() const {
        return has_type(CPLXSXP);
    }

    bool is_complex_literal() {
        return is_complex_vector() && get_length() == 1;
    }

    Rcomplex get_complex_element(int index) {
        return COMPLEX(get_value())[index];
    }

    bool has_complex_value(Rcomplex value) {
        Rcomplex value2 =
            get_complex_element(get_index_interval().get_minimum());
        return value.r == value2.r && value.i == value2.i;
    }

    /***************************************************************************
     * RAW
     **************************************************************************/

    bool is_raw_vector() const {
        return has_type(RAWSXP);
    }

    bool is_raw_literal() {
        return is_raw_vector() && get_length() == 1;
    }

    Rbyte get_raw_element(int index) {
        return RAW_ELT(get_value(), index);
    }

    bool has_raw_value(Rbyte value) {
        return value == get_raw_element(get_index_interval().get_minimum());
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

    /***************************************************************************
     * SYMBOL
     **************************************************************************/

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

  private:
    SEXP r_value_;
    Interval index_interval_;
};

} // namespace matchr

#endif /* MATCHR_R_VALUE_H */
