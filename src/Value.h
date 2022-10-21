#ifndef MATCHR_VALUE_H
#define MATCHR_VALUE_H

#include "Rincludes.h"

class Value {
  public:
    explicit Value(SEXP r_value): r_value_(r_value), type_(TYPEOF(r_value)) {
    }

    virtual ~Value() {
    }

    virtual Value* clone() const {
        return new Value(get_value());
    }

    SEXP get_value() const {
        return r_value_;
    }

    SEXPTYPE get_type() const {
        return type_;
    }

    virtual int get_size() const {
        return Rf_length(r_value_);
    }

    virtual bool is_slice() const {
        return false;
    }

    int get_lgl(int index) const {
        return LOGICAL(get_value())[this->tf_index(index)];
    }

    int get_int(int index) const {
        return INTEGER(get_value())[this->tf_index(index)];
    }

    double get_dbl(int index) const {
        return REAL(get_value())[this->tf_index(index)];
    }

    virtual int tf_index(int index) const {
        return index;
    }

    bool is_lgl1(int val) {
        return (get_type() == LGLSXP) && (this->get_size() == 1) &&
               (get_lgl(0) == val);
    }

    bool is_int1(int val) {
        return (get_type() == INTSXP) && (this->get_size() == 1) &&
               (get_int(0) == val);
    }

    bool is_dbl1(double val) {
        return (get_type() == REALSXP) && (this->get_size() == 1) &&
               (get_dbl(0) == val);
    }

    Value* take(int size) const;

    Value* drop(int size) const;

    bool is_equal(Value* other) const;

    SEXP to_sexp() const;

  private:
    SEXP r_value_;
    SEXPTYPE type_;
};

class SliceValue: public Value {
  public:
    SliceValue(SEXP r_value, int left, int right)
        : Value(r_value), left_(left), right_(right) {
    }

    virtual Value* clone() const override {
        return new SliceValue(get_value(), left_, right_);
    }

    bool is_slice() const override {
        return true;
    }

    virtual int get_size() const override {
        return right_ - left_ + 1;
    }

    virtual int tf_index(int index) const override {
        return Value::tf_index(index + left_);
    }

  private:
    int left_;
    int right_;
};

#endif /* MATCHR_VALUE_H */
