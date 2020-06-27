#ifndef MATCHR_INTERVAL_H
#define MATCHR_INTERVAL_H

#include <algorithm>

namespace matchr {

class Interval {
  public:
    Interval(bool open = false)
        : Interval(open, Interval::MINIMUM, Interval::MAXIMUM) {
    }

    Interval(bool open, int minimum, int maximum)
        : open_(open), minimum_(minimum), maximum_(maximum) {
    }

    bool is_open() const {
        return open_;
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

    void add(const Interval& interval) {
        minimum_ = std::min(minimum_, interval.get_minimum());
        maximum_ = std::min(maximum_, interval.get_maximum());
    }

    static const int MINIMUM;
    static const int MAXIMUM;

  private:
    bool open_;
    int minimum_;
    int maximum_;
};

} // namespace matchr

#endif /* MATCHR_INTERVAL_H */
