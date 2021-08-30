#ifndef MATCHR_RANGE_H
#define MATCHR_RANGE_H

#include <algorithm>

class Range {
  public:
    Range(int minimum, int maximum): minimum_(minimum), maximum_(maximum) {
    }

    int get_minimum() const {
        return minimum_;
    }

    Range with_minimum(int minimum) const {
        return Range(minimum, maximum_);
    }

    int get_maximum() const {
        return maximum_;
    }

    Range with_maximum(int maximum) const {
        return Range(minimum_, maximum);
    }

    int get_length() const {
        return maximum_ - minimum_;
    }

    bool inside(const Range& range) const {
        return (minimum_ >= range.get_minimum()) &&
               (maximum_ <= range.get_maximum());
    }

    bool contains(int value) const {
        return value >= minimum_ && value <= maximum_;
    }

    Range add(const Range& range) {
        return Range(minimum_ + range.get_minimum(),
                     maximum_ + range.get_maximum());
    }

    Range intersect(const Range& range) const {
        if (minimum_ > range.get_maximum() || range.get_minimum() > maximum_) {
            return Range(0, 0);
        }

        else if (inside(range)) {
            return *this;
        }

        else if (range.inside(*this)) {
            return range;
        }

        else if (minimum_ > range.get_minimum()) {
            return Range(minimum_, range.get_maximum());
        }

        else {
            return Range(range.get_minimum(), maximum_);
        }
    }

    Range unite(const Range& range) const {
        int minimum = std::min(minimum_, range.get_minimum());
        int maximum = std::min(maximum_, range.get_maximum());
        return Range(minimum, maximum);
    }

    static const int MINIMUM;
    static const int MAXIMUM;

  private:
    int minimum_;
    int maximum_;
};

#endif /* MATCHR_RANGE_H */
