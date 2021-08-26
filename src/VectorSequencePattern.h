#ifndef MATCHR_VECTOR_SEQUENCE_PATTERN_H
#define MATCHR_VECTOR_SEQUENCE_PATTERN_H

#include "SequencePattern.h"
#include "Context.h"
#include <unordered_map>

class VectorSequencePattern: public SequencePattern {
  public:
    explicit VectorSequencePattern(SEXP r_expression,
                                   const std::vector<Pattern*>& patterns)
        : SequencePattern(r_expression, patterns) {
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        clone.set_failure();

        return clone;
    }
};

#endif /* MATCHR_VECTOR_SEQUENCE_PATTERN_H */
