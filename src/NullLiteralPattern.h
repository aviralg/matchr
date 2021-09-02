#ifndef MATCHR_NULL_LITERAL_PATTERN_H
#define MATCHR_NULL_LITERAL_PATTERN_H

#include "LiteralPattern.h"
#include <cstring>
#include <cstdlib>

class NullLiteralPattern: public LiteralPattern {
  public:
    NullLiteralPattern(SEXP r_expression): LiteralPattern(r_expression) {
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        return Context(value.get_type() == NILSXP);
    }

  private:
    char* value_;
};

#endif /* MATCHR_NULL_LITERAL_PATTERN_H */
