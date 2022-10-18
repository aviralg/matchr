#ifndef MATCHR_SUBSET_R_VALUE_H
#define MATCHR_SUBSET_R_VALUE_H

#include "value.h"

class SubsetRValue: public RValue {
  public:
    SubsetRValue(RValue* rvalue, int begin, int end)
        : RValue(rvalue), begin_(begin), end_(end) {
    }

    int get_length() override {
        return end_ - begin_;
    }

  protected:
    int transform_index_(int index) override {
        int new_index = begin_ + index;
        if (new_index >= end_) {
            Rf_error("out of bounds access");
        }

        return new_index;
    }

  private:
    int begin_;
    int end_;
};

#endif /* MATCHR_SUBSET_R_VALUE */
