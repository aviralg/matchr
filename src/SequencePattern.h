#ifndef MATCHR_SEQUENCE_PATTERN_H
#define MATCHR_SEQUENCE_PATTERN_H

#include "VariadicPattern.h"
#include "Context.h"
#include <unordered_map>

class SequencePattern: public VariadicPattern {
  public:
    explicit SequencePattern(SEXP r_expression,
                             const std::vector<Pattern*>& patterns)
        : VariadicPattern(r_expression, patterns) {
    }
};

#endif /* MATCHR_SEQUENCE_PATTERN_H */
