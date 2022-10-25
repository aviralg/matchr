#ifndef MATCHR_VALUE_H
#define MATCHR_VALUE_H

#include "Rincludes.h"
#include "utils.h"
#include <vector>

class Value {
  public:
    explicit Value() {
    }

    virtual ~Value() {
    }

    virtual Value* clone() const = 0;

    virtual SEXPTYPE get_type() const = 0;

    virtual int get_size() const = 0;

    virtual bool is_slice() const = 0;

    virtual int get_lgl(int index) const = 0;

    virtual int get_int(int index) const = 0;

    virtual double get_dbl(int index) const = 0;

    virtual const char* get_str(int index) const = 0;

    virtual Rcomplex get_cpx(int index) const = 0;

    virtual Rbyte get_raw(int index) const = 0;

    virtual Value* get_elt(int index) const = 0;

    bool is_na1() const {
        return is_lgl1(NA_LOGICAL) || is_int1(NA_INTEGER) || is_dbl1(NA_REAL) ||
               is_str1(nullptr) || is_cpx1(Rcomplex{NA_REAL, NA_REAL});
    }

    bool is_lgl1(int val) const {
        return (this->get_type() == LGLSXP) && (this->get_size() == 1) &&
               (this->get_lgl(0) == val);
    }

    bool is_int1(int val) const {
        return (this->get_type() == INTSXP) && (this->get_size() == 1) &&
               (this->get_int(0) == val);
    }

    bool is_dbl1(double val) const {
        return (this->get_type() == REALSXP) && (this->get_size() == 1) &&
               cmp_dbl(this->get_dbl(0), val);
    }

    bool is_str1(const char* val) const {
        return (this->get_type() == STRSXP) && (this->get_size() == 1) &&
               cmp_str(this->get_str(0), val);
    }

    bool is_cpx1(const Rcomplex& val) const {
        return (this->get_type() == CPLXSXP) && (this->get_size() == 1) &&
               cmp_cpx(this->get_cpx(0), val);
    }

    bool is_raw1(Rbyte val) const {
        return (this->get_type() == RAWSXP) && (this->get_size() == 1) &&
               (this->get_raw(0) == val);
    }

    Value* take(int size) const;

    Value* drop(int size) const;

    Value* pick(int index) const;

    bool is_equal(Value* other) const;

    SEXP to_sexp() const;
};

class RawValue: public Value {
  public:
    explicit RawValue(SEXP r_value): r_value_(r_value) {
    }

    virtual ~RawValue() {
    }

    virtual Value* clone() const override {
        return new RawValue(r_value_);
    }

    SEXP get_value() const {
        return r_value_;
    }

    virtual SEXPTYPE get_type() const override {
        return TYPEOF(r_value_);
    }

    virtual int get_size() const override {
        return Rf_length(r_value_);
    }

    virtual bool is_slice() const override {
        return false;
    }

    virtual int get_lgl(int index) const override {
        return LOGICAL(get_value())[index];
    }

    virtual int get_int(int index) const override {
        return INTEGER(get_value())[index];
    }

    virtual double get_dbl(int index) const override {
        return REAL(get_value())[index];
    }

    virtual const char* get_str(int index) const override {
        return str_unwrap(STRING_ELT(get_value(), index));
    }

    virtual Rcomplex get_cpx(int index) const override {
        return COMPLEX(get_value())[index];
    }

    virtual Rbyte get_raw(int index) const override {
        return RAW_ELT(get_value(), index);
    }

    virtual Value* get_elt(int index) const override {
        return new RawValue(VECTOR_ELT(r_value_, index));
    }

  private:
    SEXP r_value_;
};

class SliceValue: public Value {
  public:
    SliceValue(const Value* value, int left, int right)
        : Value(), value_(value->clone()), left_(left), right_(right) {
    }

    virtual ~SliceValue() {
        delete value_;
    }

    virtual Value* clone() const override {
        return new SliceValue(value_->clone(), left_, right_);
    }

    bool is_slice() const override {
        return true;
    }

    virtual int get_size() const override {
        return right_ - left_ + 1;
    }

    virtual SEXPTYPE get_type() const override {
        return value_->get_type();
    }

    virtual int get_lgl(int index) const override {
        return value_->get_lgl(tf_index_(index));
    }

    virtual int get_int(int index) const override {
        return value_->get_int(tf_index_(index));
    }

    virtual double get_dbl(int index) const override {
        return value_->get_dbl(tf_index_(index));
    }

    virtual const char* get_str(int index) const override {
        return value_->get_str(tf_index_(index));
    }

    virtual Rcomplex get_cpx(int index) const override {
        return value_->get_cpx(tf_index_(index));
    }

    virtual Rbyte get_raw(int index) const override {
        return value_->get_raw(tf_index_(index));
    }

    virtual Value* get_elt(int index) const override {
        return value_->get_elt(tf_index_(index));
    }

  private:
    Value* value_;
    int left_;
    int right_;

    int tf_index_(int index) const {
        return index + left_;
    }
};

class RangeValue: public Value {
  public:
    RangeValue(): Value() {
    }

    virtual ~RangeValue() {
        for (int i = 0; i < values_.size(); ++i) {
            delete values_[i];
        }
    }

    virtual Value* clone() const override {
        RangeValue* value = new RangeValue();

        int size = values_.size();
        for (int i = 0; i < size; ++i) {
            value->push_back(values_[i]);
        }

        return value;
    }

    void push_back(const Value* value) {
        values_.push_back(value->clone());
    }

    bool is_slice() const override {
        return true;
    }

    virtual int get_size() const override {
        return values_.size();
    }

    virtual SEXPTYPE get_type() const override {
        return VECSXP;
    }

    virtual int get_lgl(int index) const override {
        Rf_error("Value::get_lgl not valid");
        return NA_LOGICAL;
    }

    virtual int get_int(int index) const override {
        Rf_error("Value::get_int not valid");
        return NA_INTEGER;
    }

    virtual double get_dbl(int index) const override {
        Rf_error("Value::get_dbl not valid");
        return NA_REAL;
    }

    virtual const char* get_str(int index) const override {
        Rf_error("Value::get_str not valid");
        return nullptr;
    }

    virtual Rcomplex get_cpx(int index) const override {
        Rf_error("Value::get_cpx not valid");
        return Rcomplex{NA_REAL, NA_REAL};
    }

    virtual Rbyte get_raw(int index) const override {
        Rf_error("Value::get_raw not valid");
        return 255;
    }

    virtual Value* get_elt(int index) const override {
        return values_[index];
    }

  private:
    std::vector<Value*> values_;
};

class ComplexValue: public Value {
  public:
    ComplexValue(const Value* value, bool real)
        : Value(), value_(value->clone()), real_(real) {
    }

    virtual ~ComplexValue() {
        delete value_;
    }

    virtual Value* clone() const override {
        return new ComplexValue(value_->clone(), real_);
    }

    bool is_slice() const override {
        return value_->is_slice();
    }

    virtual int get_size() const override {
        return value_->is_slice();
    }

    virtual SEXPTYPE get_type() const override {
        return REALSXP;
    }

    virtual int get_lgl(int index) const override {
        Rf_error("Value::get_lgl not valid");
        return NA_LOGICAL;
    }

    virtual int get_int(int index) const override {
        Rf_error("Value::get_int not valid");
        return NA_INTEGER;
    }

    virtual double get_dbl(int index) const override {
        Rcomplex val = value_->get_cpx(index);
        return real_ ? val.r : val.i;
    }

    virtual const char* get_str(int index) const override {
        Rf_error("Value::get_str not valid");
        return nullptr;
    }

    virtual Rcomplex get_cpx(int index) const override {
        Rf_error("Value::get_cpx not valid");
        return Rcomplex{NA_REAL, NA_REAL};
    }

    virtual Rbyte get_raw(int index) const override {
        Rf_error("Value::get_raw not valid");
        return 255;
    }

    virtual Value* get_elt(int index) const override {
        Rf_error("Value::get_elt not valid");
        return nullptr;
    }

  private:
    Value* value_;
    bool real_;
};

#endif /* MATCHR_VALUE_H */
