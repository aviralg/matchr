#ifndef MATCHR_MATCH_H
#define MATCHR_MATCH_H

#include "pattern.h"
#include "input.h"
#include "context.h"

context_t match(pattern_t pattern, input_t  input, SEXP r_env);

#endif /* MATCHR_MATCH_H */
