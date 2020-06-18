#ifndef MATCHR_VARIADIC_PATTERN_H
#define MATCHR_VARIADIC_PATTERN_H

#include "Pattern.h"
#include <vector>

namespace matchr {

class VariadicPattern: public Pattern {
  public:
    explicit VariadicPattern(SEXP r_expression, SEXP r_environment)
        : Pattern(r_expression, r_environment) {
    }

    virtual ~VariadicPattern() {
    }

    void add_sub_pattern(PatternSPtr pattern) {
        sub_patterns_.push_back(pattern);
    }

    int get_sub_pattern_count() const {
        return sub_patterns_.size();
    }

    PatternSPtr get_sub_pattern(int index) {
        return sub_patterns_[index];
    }

    const PatternSPtr get_sub_pattern(int index) const {
        return sub_patterns_.at(index);
    }

  private:
    std::vector<PatternSPtr> sub_patterns_;
};

} // namespace matchr

#endif /* MATCHR_VARIADIC_PATTERN_H */
