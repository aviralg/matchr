#ifndef MATCHR_R_VALUE_H
#define MATCHR_R_VALUE_H

#include <string>
#include "r.h"

class RValue {
  public:
    RValue(RValue* inner): inner_(inner) {
    }

    virtual SEXP get_value() const {
        return inner_->get_value();
    }

    virtual SEXPTYPE get_type() const {
        return inner_->get_type();
    }

    virtual bool has_type(SEXPTYPE type) const {
        return inner_->has_type(type);
    }

    virtual int get_length() const {
        return inner_->get_length();
    }

    virtual bool has_length(int size) const {
        return inner_->has_length(size);
    }

    /***************************************************************************
     * VECTOR
     **************************************************************************/

    virtual bool is_vector() const {
        return inner_->is_vector();
    }

    virtual bool is_scalar() const {
        return inner_->is_scalar();
    }

    /***************************************************************************
     * CHARACTER
     **************************************************************************/

    virtual bool is_character_vector() const {
        return inner_->is_character_vector();
    }

    virtual bool is_character_scalar() {
        return inner_->is_character_scalar();
    }

    virtual std::string get_character_element(int index) {
        int new_index = transform_index_(index);
        return inner_->get_character_element(new_index);
    }

    /***************************************************************************
     * LOGICAL
     **************************************************************************/

    virtual bool is_logical_vector() const {
        return inner_->is_logical_vector();
    }

    virtual bool is_logical_scalar() {
        return inner_->is_logical_scalar();
    }

    virtual int get_logical_element(int index) {
        int new_index = transform_index_(index);
        return inner_->get_logical_element(new_index);
    }

    /***************************************************************************
     * INTEGER
     **************************************************************************/

    virtual bool is_integer_vector() const {
        return inner_->is_integer_vector();
    }

    virtual bool is_integer_scalar() {
        return inner_->is_integer_scalar();
    }

    virtual int get_integer_element(int index) {
        int new_index = transform_index_(index);
        return inner_->get_integer_element(new_index);
    }

    /***************************************************************************
     * REAL
     **************************************************************************/

    virtual bool is_real_vector() const {
        return inner_->is_real_vector();
    }

    virtual bool is_real_scalar() {
        return inner_->is_real_scalar();
    }

    virtual double get_real_element(int index) {
        int new_index = transform_index_(index);
        return inner_->get_real_element(new_index);
    }

    /***************************************************************************
     * COMPLEX
     **************************************************************************/

    virtual bool is_complex_vector() const {
        return inner_->is_complex_vector();
    }

    virtual bool is_complex_scalar() {
        return inner_->is_complex_scalar();
    }

    virtual const Rcomplex& get_complex_element(int index) {
        int new_index = transform_index_(index);
        return inner_->get_complex_element(new_index);
    }

    /***************************************************************************
     * RAW
     **************************************************************************/

    virtual bool is_raw_vector() const {
        return inner_->is_raw_vector();
    }

    virtual bool is_raw_scalar() {
        return inner_->is_raw_scalar();
    }

    virtual Rbyte get_raw_element(int index) {
        int new_index = transform_index_(index);
        return inner_->get_raw_element(new_index);
    }

    /***************************************************************************
     * LIST
     **************************************************************************/

    virtual bool is_list() const {
        return inner_->is_list();
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

  private:
    RValue* inner_;

    virtual int transform_index_(int index) {
        return inner_->transform_index_(index);
    }
};

#endif /* MATCHR_R_VALUE_H */
