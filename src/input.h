#ifndef MATCHR_INPUT_H
#define MATCHR_INPUT_H

#include <string>
#include "r.h"
#include "output.h"

struct input_impl_t;

typedef input_impl_t* input_t;

namespace input {

input_t from_sexp(SEXP r_value);

input_t from_subset(input_t input, int index, int length);

input_t from_permutation(input_t input, const std::vector<int>& indices);

input_t from_complex(input_t input, bool real);

void destroy(input_t input, bool deep = false);

SEXP get_sexp(input_t input);

SEXPTYPE get_type(input_t input);

int get_length(input_t input);

bool is_integer_scalar(input_t input, int expected);

int get_integer_element(input_t input, int index);

bool is_logical_scalar(input_t input, int expected);

int get_logical_element(input_t input, int index);

bool is_raw_scalar(input_t input, Rbyte expected);

Rbyte get_raw_element(input_t input, int index);

bool is_character_scalar(input_t input, SEXP expected);

SEXP get_character_element(input_t input, int index);

bool is_real_scalar(input_t input, double expected);

double get_real_element(input_t input, int index);

bool is_complex_scalar(input_t input, const Rcomplex& expected);

Rcomplex get_complex_element(input_t input, int index);

input_t get_list_element(input_t input, int index);

input_t get_closure_formals(input_t input);

input_t get_closure_body(input_t input);

input_t get_closure_environment(input_t input);

input_t get_language_element(input_t input, int index);

output_t to_output(input_t input);

} // namespace input

#endif /* MATCHR_INPUT_H */
