#ifndef MATCHR_RANGE_H
#define MATCHR_RANGE_H

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

    static const int MINIMUM;

    static const int MAXIMUM;

  private:
    int minimum_;
    int maximum_;
};

} // namespace matchr

#endif /* MATCHR_RANGE_H */
