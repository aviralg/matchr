#ifndef MATCHR_RANGE_H
#define MATCHR_RANGE_H

#include <algorithm>

namespace matchr {

class Range {
  public:
    Range(): Range(Range::MINIMUM, Range::MAXIMUM) {
    }

    Range(int minimum, int maximum): minimum_(minimum), maximum_(maximum) {
    }

    int get_minimum() const {
        return minimum_;
    }

    void set_minimum(int minimum) {
        minimum_ = minimum;
    }

    int get_maximum() const {
        return maximum_;
    }

    void set_maximum(int maximum) {
        maximum_ = maximum;
    }

    void set(int minimum, int maximum) {
        set_minimum(minimum);
        set_maximum(maximum);
    }

    void set(int value) {
        set(value, value);
    }

    void add(const Range& range) {
        minimum_ = std::min(minimum_, range.get_minimum());
        maximum_ = std::min(maximum_, range.get_maximum());
    }

    static const int MINIMUM;

    static const int MAXIMUM;

  private:
    int minimum_;
    int maximum_;
};

} // namespace matchr

#endif /* MATCHR_RANGE_H */
