#include "output.h"

enum class output_type_t { Whole, Partial, Components };

struct output_t {
    output_type_t type;
    union {
        struct {
            SEXP r_value;
        } whole;

        struct {
            SEXP r_value;
            std::vector<int> indices;
            int real;
        } partial;

        struct {
            SEXPTYPE type;
            std::vector<output_t> outputs;
        } components;
    } data;
};

namespace output {

output_t from_whole(SEXP r_value) {
    output_t output = new output_impl_t();
    output->type = output_type_t::Whole;
    output->whole.r_value = r_value;
    return output;
}

output_t from_partial(SEXP r_value, const std::vector<int>& indices, int real) {
    output_t output = new output_impl_t();
    output->type = output_type_t::Partial;
    output->partial.r_value = r_value;
    output->partial.indices = indices;
    output->partial.real = real;
    return output;
}

output_t from_components(SEXPTYPE type, const std::vector<output_t>& outputs) {
    output_t output = new output_impl_t();
    output->type = output_type_t::Components;
    output->components.type = type;
    output->components.outputs = outputs;
    return output;
}

void destroy(output_t output, bool deep) {
    switch (output->type) {
    case output_type_t::Whole:
        delete output;
        return;
    case output_type_t::Partial:
        delete output;
        break;
    case output_type_t::Components:
        for (int i = 0; deep && i < output.components.outputs.size(); ++i) {
            destroy(output.components.outputs[i], deep);
        }
        delete output;
        break;
    default:
        Rf_error("unhandled output type in output::destroy");
        break;
    }
}

SEXP to_sexp_partial(SEXP src,
                     const std::vector<int>& indices,
                     const int real) {
    SEXP dst = R_NilValue;
    SEXPTYPE type = TYPEOF(src);
    int length = indices.size();

    switch (type) {
    case INTSXP:
        dst = PROTECT(Rf_allocVector(INTSXP, length));
        for (int i = 0; i < length; ++i) {
            SET_INTEGER_ELT(dst, i, INTEGER_ELT(src, indices[i]));
        }
        UNPROTECT(1);
        break;

    case REALSXP:
        dst = PROTECT(Rf_allocVector(REALSXP, length));
        for (int i = 0; i < length; ++i) {
            SET_REAL_ELT(dst, i, REAL_ELT(src, indices[i]));
        }
        UNPROTECT(1);
        break;

    case LGLSXP:
        dst = PROTECT(Rf_allocVector(LGLSXP, length));
        for (int i = 0; i < length; ++i) {
            SET_LOGICAL_ELT(dst, i, LOGICAL_ELT(src, indices[i]));
        }
        UNPROTECT(1);
        break;

    case CPLXSXP:
        if (real == -1) {
            dst = PROTECT(Rf_allocVector(CPLXSXP, length));
            for (int i = 0; i < length; ++i) {
                SET_COMPLEX_ELT(dst, i, COMPLEX_ELT(src, indices[i]));
            }
            UNPROTECT(1);
        }

        else if (real == 0) {
            dst = PROTECT(Rf_allocVector(REALSXP, length));
            for (int i = 0; i < length; ++i) {
                SET_REAL_ELT(dst, i, COMPLEX_ELT(src, indices[i]).i);
            }
            UNPROTECT(1);
        }

        else if (real == 1) {
            dst = PROTECT(Rf_allocVector(REALSXP, length));
            for (int i = 0; i < length; ++i) {
                SET_REAL_ELT(dst, i, COMPLEX_ELT(src, indices[i]).r);
            }
            UNPROTECT(1);
        }
        break;

    case RAWSXP:
        dst = PROTECT(Rf_allocVector(RAWSXP, length));
        for (int i = 0; i < length; ++i) {
            SET_RAW_ELT(dst, i, RAW_ELT(src, indices[i]));
        }
        UNPROTECT(1);
        break;

    case STRSXP:
        dst = PROTECT(Rf_allocVector(STRSXP, length));
        for (int i = 0; i < length; ++i) {
            SET_STRING_ELT(dst, i, STRING_ELT(src, indices[i]));
        }
        UNPROTECT(1);
        break;

    default:
        Rf_error("Unknown type encountered by to_sexp_partial");
        break;
    }

    return dst;
}

SEXP to_sexp_components(SEXPTYPE type, const std::vector<output_t>& outputs) {
    int length = outputs.size();

    switch (type) {
    case VECSXP:
        dst = PROTECT(allocVector(VECSXP, length));
        for (int i = 0; i < outputs.size(); ++i) {
            SET_VECTOR_ELT(dst, i, to_sexp(outputs[i]));
        }
        UNPROTECT(1);
        break;

    default:
        Rf_error("Unknown type encountered by to_sexp_components");
        break;
    }

    return dst;
}

SEXP to_sexp(const output_t& output) {
    output_type_t type = output->type;

    switch (type) {
    case output_type_t::Whole:
        return output->whole.r_value;
        break;
    case output_type_t::Partial:
        return to_sexp_partial(output->partial.r_value,
                               output->partial.indices,
                               output->partial.real);
        break;
    case output_type_t::Components:
        return to_sexp_components(output->components.type,
                                  output->components.outputs);
        break;
    default:
        Rf_error("Unhandled output::to_sexp type");
        return NULL;
        break;
    }
}

/*

bool is_equal_integer(SEXP r_value_a,
                      const std::vector<int>& indices_a,
                      SEXP r_value_b,
                      const std::vector<int>& indices_b) {
    int length = indices_a.size();

    for (int i = 0; i < length; ++i) {
        if (INTEGER_ELT(r_value_a, indices_a[i]) !=
            INTEGER_ELT(r_value_b, indices_b[i]))
            return false;
    }
    return true;
}

bool is_equal_real(SEXP r_value_a,
                   const std::vector<int>& indices_a,
                   SEXP r_value_b,
                   const std::vector<int>& indices_b) {
    int length = indices_a.size();

    for (int i = 0; i < length; ++i) {
        const double a = REAL_ELT(r_value_a, indices_a[i]);
        const double b = REAL_ELT(r_value_b, indices_b[i]);

        if (!is_equal_double(a, b)) {
            return false;
        }
    }
    return true;

    return is_equal_double(double_a, double_b);
}

bool is_equal_logical(SEXP r_value_a,
                      const std::vector<int>& indices_a,
                      SEXP r_value_b,
                      const std::vector<int>& indices_b) {
    int length = indices_a.size();

    for (int i = 0; i < length; ++i) {
        if (LOGICAL_ELT(r_value_a, indices_a[i]) !=
            LOGICAL_ELT(r_value_b, indices_b[i]))
            return false;
    }
    return true;
}

bool is_equal_raw(SEXP r_value_a,
                  const std::vector<int>& indices_a,
                  SEXP r_value_b,
                  const std::vector<int>& indices_b) {
    int length = indices_a.size();

    for (int i = 0; i < length; ++i) {
        if (RAW_ELT(r_value_a, indices_a[i]) !=
            RAW_ELT(r_value_b, indices_b[i]))
            return false;
    }
    return true;
}

bool is_equal_character(SEXP r_value_a,
                        const std::vector<int>& indices_a,
                        SEXP r_value_b,
                        const std::vector<int>& indices_b) {
    int length = indices_a.size();

    for (int i = 0; i < length; ++i) {
        SEXP r_char_a = STRING_ELT(r_value_a, indices_a[i]);
        SEXP r_char_b = STRING_ELT(r_value_b, indices_b[i]);

        if (!is_equal_char(r_char_a, r_char_b)) {
            return false;
        }
    }

    return true;
}

bool is_equal_complex(SEXP r_value_a,
                      int index_a,
                      SEXP r_value_b,
                      int index_b) {
    const RComplex& complex_a = COMPLEX_ELT(r_value_a, index_a);
    const RComplex& complex_b = COMPLEX_ELT(r_value_b, index_b);

    return is_equal_double(complex_a.r, complex_b.r) &&
           is_equal_double(complex_a.i, complex_b.i);
}

bool is_equal_complex_and_real(SEXP value_a,
                               const std::vector<int>& indices_a,
                               const int real_a,
                               SEXP value_b,
                               const std::vector<int>& indices_b) {
    int length = indices_a.size();

    if (real_b == 0) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_double(COMPLEX_ELT(value_a, indices_a[i]).i,
                                 REAL_ELT(value_b, indices_b[i]))) {
                return false;
            }
        }
        return true;
    } else {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_double(COMPLEX_ELT(value_a, indices_a[i]).r,
                                 REAL_ELT(value_b, indices_b[i]))) {
                return false;
            }
        }
        return true;
    }
}

bool is_equal_complex_and_complex(SEXP value_a,
                                  const std::vector<int>& indices_a,
                                  const int real_a,
                                  SEXP value_b,
                                  const std::vector<int>& indices_b,
                                  const int real_b) {
    int length = indices_a.size();

    if (real_a == real_b && real_a == 0) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_double(COMPLEX_ELT(value_a, indices_a[i]).i,
                                 COMPLEX_ELT(value_b, indices_b[i]).i)) {
                return false;
            }
        }
        return true;
    } else if (real_a == real_b && real_a == 1) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_double(COMPLEX_ELT(value_a, indices_a[i]).r,
                                 COMPLEX_ELT(value_b, indices_b[i]).r)) {
                return false;
            }
        }
        return true;
    } else if (real_a == 0) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_double(COMPLEX_ELT(value_a, indices_a[i]).i,
                                 COMPLEX_ELT(value_b, indices_b[i]).r)) {
                return false;
            }
        }
        return true;
    } else {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_double(COMPLEX_ELT(value_a, indices_a[i]).r,
                                 COMPLEX_ELT(value_b, indices_b[i]).i)) {
                return false;
            }
        }
        return true;
    }
}

bool is_equal(output_t output_a, output_t output_b) {
    // if the value pointers are same, then the values are equal
    if (output_a == output_b) {
        return true;
    }

    const int real_a = output_a.real;
    const std::vector<int>& indices_a = output_a.indices;
    SEXP r_value_a = output_a.r_value;
    SEXPTYPE sexptype_a = real_a != -1 ? REALSXP : TYPEOF(r_value_a);

    output_t output_b = compute_output(value_b);
    const int real_b = output_b.real;
    const std::vector<int>& indices_b = output_b.indices;
    SEXP r_value_b = output_b.r_value;
    SEXPTYPE sexptype_b = real_b != -1 ? REALSXP : TYPEOF(r_value_b);

    // if the two sequences have different type, they are unequal
    if (sexptype_a != sexptype_b) {
        return false;
    }

    int length = indices_a.size();

    // if the two sequences have different lengths, they are unequal
    if (length != indices_b.size()) {
        return false;
    }

    switch (sexptype_a) {
    case INTSXP:
        return is_equal_integer(r_value_a, indices_a, r_value_b, indices_b);
        break;

    case REALSXP:
        // both values are true REALSXP
        if (real_a == -1 && real_b == -1) {
            return is_equal_real(r_value_a, indices_a, r_value_b, indices_b);
        }
        // value_b is CPLXSXP
        else if (real_a == -1) {
            return is_equal_complex_and_real(
                value_b, indices_b, real_b, value_a, indices_a);

        }

        // value_a is CPLXSXP
        else if (real_b == -1) {
            return is_equal_complex_and_real(
                value_a, indices_a, real_a, value_b, indices_b);
        }

        // both value_a and value_b are CPLXSXP
        else {
            return is_equal_complex_and_complex(
                value_a, indices_a, real_a, value_b, indices_b, real_b);
        }

        break;

    case LGLSXP:
        return is_equal_logical(r_value_a, indices_a, r_value_b, indices_b);
        break;

    case CPLXSXP:
        return is_equal_complex(r_value_a, indices_a, r_value_b, indices_b);
        break;

    case RAWSXP:
        return is_equal_raw(r_value_a, indices_a, r_value_b, indices_b);
        break;

    case STRSXP:
        return is_equal_character(r_value_a, indices_a, r_value_b, indices_b);
        break;

    default:
        Rf_error("Unknown type '%s' encountered by to_sexp",
                 type2str(sexptype_a));
    }
}
*/

} // namespace output

// RValue extract(int index) {
//    if (is_vector()) {
//        return subset(index, 1);
//    } else if (get_type() == LANGSXP) {
//        return get_language_element(index);
//    } else {
//        /* TODO: support lists, envs, etc. */
//        Rf_error("extract not supported for non vector values");
//    }
//}
