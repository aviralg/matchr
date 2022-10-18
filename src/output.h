#ifndef MATCHR_OUTPUT_H
#define MATCHR_OUTPUT_H

#include <string>
#include "r.h"

struct output_impl_t;

typedef output_impl_t* output_t;

namespace output {

output_t from_whole(SEXP r_value);

output_t from_partial(SEXP r_value, const std::vector<int>& indices, int real);

output_t from_components(SEXPTYPE type, const std::vector<output_t>& outputs);

void destroy(output_t output, bool deep = false);

SEXP to_sexp(output_t output);

// bool is_equal(output_t output_a, output_t output_b);

} // namespace output

#endif /* MATCHR_OUTPUT_H */
