#ifndef MATCHR_SEQUENCE_PATTERN_H
#define MATCHR_SEQUENCE_PATTERN_H

#include "Pattern.h"
#include "Context.h"
#include <vector>

namespace matchr {

class SequencePattern: public Pattern {
  public:
    explicit SequencePattern(SEXP expression): Pattern(expression) {
    }

    virtual ~SequencePattern() {
        for (int index = 0; index < get_pattern_count(); ++index) {
            delete patterns_[index];
        }
    }

    void add_pattern(Pattern* pattern) {
        patterns_.push_back(pattern);
    }

    int get_pattern_count() const {
        return patterns_.size();
    }

    Pattern* get_pattern(int index) {
        return patterns_[index];
    }

    const Pattern* get_pattern(int index) const {
        return patterns_[index];
    }

  private:
    std::vector<Pattern*> patterns_;
};

} // namespace matchr

#endif /* MATCHR_SEQUENCE_PATTERN_H */
