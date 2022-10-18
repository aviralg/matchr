#ifndef MATCHR_PATTERN_H
#define MATCHR_PATTERN_H

#include "Context.h"
#include "Identifiers.h"
#include "Range.h"
#include "input.h"

struct pattern_impl_t;

typedef pattern_impl_t* pattern_t;

namespace pattern {

void destroy(pattern_t pattern);

const Identifiers& get_identifiers(pattern_t pattern);

const Range& get_range(pattern_t pattern);

SEXP get_expression(pattern_t pattern);

Context match(pattern_t pattern, input_t input, SEXP r_pat_env);

pattern_t parse(SEXP r_expression);

} // namespace pattern

#endif /* MATCHR_PATTERN_H */
