#include "input.h"
#include <vector>
#include "utilities.h"

enum class input_type_t { Sexp, Subset, Permutation, Complex };

struct input_impl_t {
    input_type_t type;

    union data_t {
        data_t() {
        }

        ~data_t() {
        }

        struct {
            SEXP r_value;
        } sexp;

        struct {
            int index;
            int length;
            input_t input;
        } subset;

        struct {
            bool real;
            input_t input;
        } complex;

        struct permutation_t {
            permutation_t() {
            }

            ~permutation_t() {
            }

            std::vector<int> indices;
            input_t input;
        } permutation;

    } data;
};

namespace input {

input_t from_sexp(SEXP r_value) {
    input_t input = new input_impl_t();
    input->type = input_type_t::Sexp;
    return input;
}

input_t from_subset(input_t input, int index, int length) {
    input_t new_input = new input_impl_t();
    new_input->type = input_type_t::Subset;
    new_input->data.subset.index = index;
    new_input->data.subset.length = length;
    new_input->data.subset.input = input;
    return new_input;
}

input_t from_permutation(input_t input, const std::vector<int>& indices) {
    input_t new_input = new input_impl_t();
    new_input->type = input_type_t::Permutation;
    new_input->data.permutation.indices = indices;
    new_input->data.permutation.input = input;
    return new_input;
}

input_t from_complex(input_t input, bool real) {
    input_t new_input = new input_impl_t();
    new_input->type = input_type_t::Complex;
    new_input->data.complex.real = real;
    new_input->data.complex.input = input;
    return new_input;
}

void destroy(input_t input, bool deep) {
    input_t next_input = nullptr;

    do {
        switch (input->type) {
        case input_type_t::Sexp:
            next_input = nullptr;
            break;
        case input_type_t::Subset:
            next_input = input->data.subset.input;
            break;
        case input_type_t::Permutation:
            next_input = input->data.permutation.input;
            break;
        case input_type_t::Complex:
            next_input = input->data.complex.input;
            break;
        default:
            Rf_error("unhandled input type in input::destroy");
            break;
        }

        delete input;
        input = next_input;

    } while (deep && input != nullptr);
}

/* this function does four things in one traversal:
1: compute the size of input
2: compute the index based on subset and permutation
3: identify the component accessed for complex numbers
4: return the base input wrapper
*/
input_t transform_index(input_t input, int& length, int& index, int& real) {
    length = -1;
    while (TRUE) {
        switch (input->type) {
        case input_type_t::Sexp:
            if (length == -1) {
                length = Rf_length(input->data.sexp.r_value);
            }
            return input;
            break;
        case input_type_t::Subset:
            input = input->data.subset.input;
            index = index + input->data.subset.index;
            if (length == -1) {
                length = input->data.subset.length;
            }
            break;
        case input_type_t::Permutation:
            input = input->data.permutation.input;
            index = input->data.permutation.indices[index];
            if (length == -1) {
                length = input->data.permutation.indices.size();
            }
            break;
        case input_type_t::Complex:
            input = input->data.complex.input;
            real = input->data.complex.real;
            break;
        default:
            Rf_error("Unhandled type in input::transform_index");
            break;
        }
    }
}

SEXP get_sexp(input_t input) {
    switch (input->type) {
    case input_type_t::Sexp:
        return input->data.sexp.r_value;
        break;
    case input_type_t::Subset:
        return get_sexp(input->data.subset.input);
        break;
    case input_type_t::Permutation:
        return get_sexp(input->data.permutation.input);
        break;
    case input_type_t::Complex:
        return get_sexp(input->data.complex.input);
        break;
    default:
        Rf_error("Unhandled type in input::get_sexp");
        break;
    }
}

SEXPTYPE get_type(input_t input) {
    switch (input->type) {
    case input_type_t::Sexp:
        return TYPEOF(input->data.sexp.r_value);
        break;
    case input_type_t::Subset:
        return get_type(input->data.subset.input);
        break;
    case input_type_t::Permutation:
        return get_type(input->data.permutation.input);
        break;
    case input_type_t::Complex:
        return REALSXP;
        break;
    default:
        Rf_error("Unhandled type in input::get_type");
        break;
    }
}

int get_length(input_t input) {
    switch (input->type) {
    case input_type_t::Sexp:
        return Rf_length(input->data.sexp.r_value);
        break;
    case input_type_t::Subset:
        return input->data.subset.length;
        break;
    case input_type_t::Permutation:
        return get_length(input->data.permutation.input);
        break;
    case input_type_t::Complex:
        return get_length(input->data.complex.input);
        break;
    default:
        Rf_error("Unhandled type in input::get_length");
        break;
    }
}

bool is_integer_scalar(input_t input, int expected) {
    int index = 0;
    int length = -1;
    int real = -1;

    input = transform_index(input, length, index, real);

    SEXP r_input = input->data.sexp.r_value;

    return (TYPEOF(r_input) == INTSXP) && length == 1 &&
           (INTEGER_ELT(r_input, index) == expected);
}

int get_integer_element(input_t input, int index) {
    int real = -1;
    int length = -1;
    input = transform_index(input, length, index, real);
    SEXP r_input = input->data.sexp.r_value;
    return INTEGER_ELT(r_input, index);
    // return is_abstract_() ?
    // elements_[new_index].get_integer_element(0)
}

bool is_logical_scalar(input_t input, int expected) {
    int index = 0;
    int length = -1;
    int real = -1;

    input = transform_index(input, length, index, real);

    SEXP r_input = input->data.sexp.r_value;

    return (TYPEOF(r_input) == LGLSXP) && length == 1 &&
           (LOGICAL_ELT(r_input, index) == expected);
}

int get_logical_element(input_t input, int index) {
    int real = -1;
    int length = -1;
    input = transform_index(input, length, index, real);
    SEXP r_input = input->data.sexp.r_value;
    return LOGICAL_ELT(r_input, index);
    // return is_abstract_() ?
    // elements_[new_index].get_logical_element(0)
}

bool is_raw_scalar(input_t input, Rbyte expected) {
    int index = 0;
    int length = -1;
    int real = -1;

    input = transform_index(input, length, index, real);

    SEXP r_input = input->data.sexp.r_value;

    return (TYPEOF(r_input) == RAWSXP) && length == 1 &&
           (RAW_ELT(r_input, index) == expected);
}

Rbyte get_raw_element(input_t input, int index) {
    int real = -1;
    int length = -1;
    input = transform_index(input, length, index, real);
    SEXP r_input = input->data.sexp.r_value;
    return RAW_ELT(r_input, index);
    // return is_abstract_() ? elements_[new_index].get_raw_element(0)
}

bool is_character_scalar(input_t input, SEXP expected) {
    int index = 0;
    int length = -1;
    int real = -1;

    input = transform_index(input, length, index, real);

    SEXP r_input = input->data.sexp.r_value;

    return (TYPEOF(r_input) == STRSXP) && length == 1 &&
           is_equal_char(expected, STRING_ELT(r_input, index));
}

SEXP get_character_element(input_t input, int index) {
    int real = -1;
    int length = -1;
    input = transform_index(input, length, index, real);
    SEXP r_input = input->data.sexp.r_value;
    return STRING_ELT(r_input, index);
    // if (is_abstract_()) {
    //    return elements_[new_index].get_character_element(0);
    //}
}

bool is_real_scalar(input_t input, double expected) {
    int index = 0;
    int length = -1;
    int real = -1;

    input = transform_index(input, length, index, real);

    SEXP r_input = input->data.sexp.r_value;

    double actual = 0.0;

    if (length != 1) {
        return false;
    }

    if (real == -1) {
        actual = REAL_ELT(r_input, index);
    } else if (real == 0) {
        actual = COMPLEX_ELT(r_input, index).i;
    } else {
        actual = COMPLEX_ELT(r_input, index).r;
    }

    return is_equal_double(expected, actual);
}

double get_real_element(input_t input, int index) {
    int real = -1;
    int length = -1;
    input = transform_index(input, length, index, real);
    SEXP r_input = input->data.sexp.r_value;
    if (real == -1) {
        return REAL_ELT(r_input, index);
    } else if (real == 0) {
        return COMPLEX_ELT(r_input, index).i;
    } else if (real == 1) {
        return COMPLEX_ELT(r_input, index).r;
    }

    // return is_abstract_() ? elements_[index].get_real_element(0)
}

bool is_complex_scalar(input_t input, const Rcomplex& expected) {
    int index = 0;
    int length = -1;
    int real = -1;

    input = transform_index(input, length, index, real);

    SEXP r_input = input->data.sexp.r_value;

    if (real != -1) {
        return false;
    }
    if ((TYPEOF(r_input) == CPLXSXP) && (length == 1)) {
        const Rcomplex& actual = COMPLEX_ELT(r_input, index);

        return is_equal_double(expected.r, actual.r) &&
               is_equal_double(expected.i, actual.i);
    }

    return false;
}

Rcomplex get_complex_element(input_t input, int index) {
    int real = -1;
    int length = -1;
    input = transform_index(input, length, index, real);
    SEXP r_input = input->data.sexp.r_value;
    return COMPLEX_ELT(r_input, index);
    // return is_abstract_() ?
    // elements_[new_index].get_complex_element(0)
}

input_t get_list_element(input_t input, int index) {
    int real = -1;
    int length = -1;
    input = transform_index(input, length, index, real);
    SEXP r_input = input->data.sexp.r_value;
    return from_sexp(VECTOR_ELT(r_input, index));
    // return is_abstract_() ? elements_[new_index]
}

input_t get_closure_formals(input_t input) {
    SEXP r_input = get_sexp(input);
    return from_sexp(FORMALS(r_input));
}

input_t get_closure_body(input_t input) {
    SEXP r_input = get_sexp(input);
    return from_sexp(BODY(r_input));
}

input_t get_closure_environment(input_t input) {
    SEXP r_input = get_sexp(input);
    return from_sexp(CLOENV(r_input));
}

input_t get_language_element(input_t input, int index) {
    int new_index = transform_index(index);
    SEXP r_input = get_sexp(input);
    // if (is_abstract_()) {
    //    return elements_[new_index];
    //}

    int cur_index = 0;
    for (SEXP r_result = r_input; r_result != R_NilInput;
         r_result = CDR(r_result), ++cur_index) {
        if (cur_index == new_index) {
            return from_sexp(CAR(r_result));
        }
    }

    Rf_error("attempt to extract element at position %d in a %d element "
             "language expression",
             new_index,
             get_length(input));
}

output_t to_output(input_t input) {
    int real = -1;
    std::vector<int> indices;
    bool permute = false;
    int index = -1;
    int length = -1;

    while (true) {
        input_type_t type = input->type;

        if (type == input_type_t::Sexp) {
            break;
        }

        else if (type == input_type_t::Subset) {
            int cur_index = input->data.subset.index;
            int cur_length = input->data.subset.length;

            if (permute) {
                for (int i = 0; i < indices.size(); ++i) {
                    indices[i] += cur_index;
                }
            } else if (index == -1) {
                index = cur_index;
                length = cur_length;
            } else {
                index += cur_index;
            }

            input = input->data.subset.input;
        }

        else if (type == input_type_t::Permutation) {
            std::vector<int> cur_indices = input->data.permutation.indices;

            if (permute) {
                for (int i = 0; i < indices.size(); ++i) {
                    indices[i] += cur_indices[indices[i]];
                }
            } else if (index == -1) {
                indices = cur_indices;
            } else {
                indices = std::vector(cur_indices.begin() + index,
                                      cur_indices.begin() + length);
            }

            permute = true;
            input = input->data.subset.input;
        }

        else if (type == input_type_t::Complex) {
            real = input->data.complex.real;
            input = input->data.complex.input;
        }
    }

    if (!permute) {
        if (size == -1 && real == -1) {
            output::from_whole(input->data.sexp.r_value);
        } else {
            if (size == -1) {
                size = Rf_length(input->data.sexp.r_value);
            }
            indices.reserve(size);
            for (int i = 0; i < size; ++i) {
                indices[i] += index + i;
            }
        }
    }

    return output_from_partial(input->data.sexp.r_value, indices, real);
}

} // namespace input

// RInput extract(int index) {
//    if (is_vector()) {
//        return subset(index, 1);
//    } else if (get_type() == LANGSXP) {
//        return get_language_element(index);
//    } else {
//        /* TODO: support lists, envs, etc. */
//        Rf_error("extract not supported for non vector inputs");
//    }
//}
