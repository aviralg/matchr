SEXP r_pattern_create_raw_literal_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_value) {
    Rbyte value = RAW_ELT(r_value, 0);
    PatternSPtr pattern =
        std::make_shared<RawLiteralPattern>(r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_complex_literal_pattern(SEXP r_expression,
                                              SEXP r_environment,
                                              SEXP r_value) {
    Rcomplex value = COMPLEX_ELT(r_value, 0);
    PatternSPtr pattern = std::make_shared<ComplexLiteralPattern>(
        r_expression, r_environment, value);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_predicate_pattern(SEXP r_expression,
                                        SEXP r_environment,
                                        SEXP r_predicate_expression) {
    PatternSPtr pattern = std::make_shared<PredicatePattern>(
        r_expression, r_environment, r_predicate_expression);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_create_range_unary_pattern(SEXP r_expression,
                                          SEXP r_environment,
                                          SEXP r_sub_pattern,
                                          SEXP r_minimum,
                                          SEXP r_maximum) {
    PatternSPtr sub_pattern = Pattern::from_sexp(r_sub_pattern);
    int minimum = asInteger(r_minimum);
    int maximum = asInteger(r_maximum);
    PatternSPtr pattern = std::make_shared<RangeUnaryPattern>(
        r_expression, r_environment, sub_pattern, minimum, maximum);
    return Pattern::to_sexp(pattern);
}

SEXP r_pattern_get_identifier_names(SEXP r_pattern) {
    PatternSPtr pattern = Pattern::from_sexp(r_pattern);
    IdentifierNames identifier_names = pattern->get_identifier_names();

    int count = identifier_names.get_count();

    SEXP r_identifier_names = PROTECT(allocVector(STRSXP, count));

    for (int index = 0; index < count; ++index) {
        SET_STRING_ELT(r_identifier_names,
                       index,
                       mkChar(identifier_names.get_name(index).c_str()));
    }

    UNPROTECT(1);

    return r_identifier_names;
}

SEXP slice_sequence(value_t value, const std::vector<int>& indices, int real) {
    SEXP src = value->sexp.r_value;
    SEXP dst = R_NilValue;
    SEXPTYPE type = TYPEOF(r_value);
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
        Rf_error("Unknown type encountered by to_sexp");
    }

    return dst;
}

SEXP to_sexp(value_t value) {
    /* if we are referring to the whole value, return as is. */
    if (value->type == value_type_t::Sexp) {
        return value->sexp.r_value;
    } else {
        view_t view = compute_view(value);
        return materialize_view(view);
    }
}

bool is_equal_integer(SEXP r_value_a,
                      int index_a,
                      SEXP r_value_b,
                      int index_b) {
    return INTEGER_ELT(r_value_a, index_a) == INTEGER_ELT(r_value_b, index_b);
}

bool is_equal_logical(SEXP r_value_a,
                      int index_a,
                      SEXP r_value_b,
                      int index_b) {
    return LOGICAL_ELT(r_value_a, index_a) == LOGICAL_ELT(r_value_b, index_b);
}

bool is_equal_raw(SEXP r_value_a, int index_a, SEXP r_value_b, int index_b) {
    return RAW_ELT(r_value_a, index_a) == RAW_ELT(r_value_b, index_b);
}

bool is_equal_character(SEXP r_value_a,
                        int index_a,
                        SEXP r_value_b,
                        int index_b) {
    SEXP r_char_a = STRING_ELT(r_value_a, index_a);
    SEXP r_char_b = STRING_ELT(r_value_b, index_b);

    if (r_char_a == NA_STRING) {
        return r_char_b == NA_STRING;
    }

    if (r_char_b == NA_STRING) {
        return false;
    }

    return !strcmp(CHAR(r_char_a), CHAR(r_char_b));
}

bool is_equal_complex(SEXP r_value_a,
                      int index_a,
                      SEXP r_value_b,
                      int index_b) {
    const RComplex& complex_a = COMPLEX_ELT(r_value_a, index_a);
    const RComplex& complex_b = COMPLEX_ELT(r_value_b, index_b);

    return is_equal_c_double(complex_a.r, complex_b.r) &&
           is_equal_c_double(complex_a.i, complex_b.i);
}

bool is_equal_double(SEXP r_value_a, int index_a, SEXP r_value_b, int index_b) {
    const double double_a = REAL_ELT(r_value_a, index_a);
    const double double_b = REAL_ELT(r_value_b, index_b);

    return is_equal_c_double(double_a, double_b);
}

bool is_equal_c_double(double a, double b) {
    if (std::isnan(a)) {
        return std::isnan(b);
    }

    if (std::isnan(b)) {
        return false;
    }

    return a == b;
}

bool compare_complex_and_real(SEXP value_a,
                              const std::vector<int>& indices_a,
                              const int real_a,
                              SEXP value_b,
                              const std::vector<int>& indices_b) {
    int length = indices_a.size();

    if (real_b == 0) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_c_double(COMPLEX_ELT(value_a, indices_a[i]).i,
                                   REAL_ELT(value_b, indices_b[i]))) {
                return false;
            }
        }
        return true;
    } else {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_c_double(COMPLEX_ELT(value_a, indices_a[i]).r,
                                   REAL_ELT(value_b, indices_b[i]))) {
                return false;
            }
        }
        return true;
    }
}

bool compare_complex_and_complex(SEXP value_a,
                                 const std::vector<int>& indices_a,
                                 const int real_a,
                                 SEXP value_b,
                                 const std::vector<int>& indices_b,
                                 const int real_b) {
    int length = indices_a.size();

    if (real_a == real_b && real_a == 0) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_c_double(COMPLEX_ELT(value_a, indices_a[i]).i,
                                   COMPLEX_ELT(value_b, indices_b[i]).i)) {
                return false;
            }
        }
        return true;
    } else if (real_a == real_b && real_a == 1) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_c_double(COMPLEX_ELT(value_a, indices_a[i]).r,
                                   COMPLEX_ELT(value_b, indices_b[i]).r)) {
                return false;
            }
        }
        return true;
    } else if (real_a == 0) {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_c_double(COMPLEX_ELT(value_a, indices_a[i]).i,
                                   COMPLEX_ELT(value_b, indices_b[i]).r)) {
                return false;
            }
        }
        return true;
    } else {
        for (int i = 0; i < length; ++i) {
            if (!is_equal_c_double(COMPLEX_ELT(value_a, indices_a[i]).r,
                                   COMPLEX_ELT(value_b, indices_b[i]).i)) {
                return false;
            }
        }
        return true;
    }
}

bool is_equal(value_t value_a, value_t value_b) {
    /* if the value pointers are same, then the values are equal */
    if (value_a == value_b) {
        return true;
    }

    /* if both value pointers are sexp wrappers and point to the same sexp, then
     * the values are equal. If they don't point to the same sexp, they can
     * still be equal if the sexp contents are equal. */
    if (value_a->type == value_b_type == value_type_t::Sexp) {
        if (value_a->sexp.r_value == value_b->sexp.r_value) {
            return true;
        }
    }

    view_t view_a = compute_view(value_a);
    const int real_a = view_a.real;
    const std::vector<int>& indices_a = view_a.indices;
    SEXP r_value_a = view_a.value->sexp.r_value;
    SEXPTYPE sexptype_a = real_a != -1 ? REALSXP : TYPEOF(r_value_a);

    view_t view_b = compute_view(value_b);
    const int real_b = view_b.real;
    const std::vector<int>& indices_b = view_b.indices;
    SEXP r_value_b = view_b.value->sexp.r_value;
    SEXPTYPE sexptype_b = real_b != -1 ? REALSXP : TYPEOF(r_value_b);

    /* if the two sequences have different type, they are unequal */
    if (sexptype_a != sexptype_b) {
        return false;
    }

    int length = indices_a.size();

    /* if the two sequences have different lengths, they are unequal */
    if (length != indices_b.size()) {
        return false;
    }

    switch (sexptype_a) {
    case INTSXP:
        for (int i = 0; i < length; ++i) {
            if (!is_equal_integer(
                    value_a, indices_a[i], value_b, indices_b[i])) {
                return false;
            }
        }
        return true;
        break;

    case REALSXP:
        /* both values are true REALSXP */
        if (real_a == -1 && real_b == -1) {
            for (int i = 0; i < length; ++i) {
                if (!is_equal_real(
                        value_a, indices_a[i], value_b, indices_b[i])) {
                    return false;
                }
            }
            return true;
        }
        /* value_b is CPLXSXP */
        else if (real_a == -1) {
            return compare_complex_and_real(
                value_b, indices_b, real_b, value_a, indices_a);

        }

        /* value_a is CPLXSXP */
        else if (real_b == -1) {
            return compare_complex_and_real(
                value_a, indices_a, real_a, value_b, indices_b);
        }

        /* both value_a and value_b are CPLXSXP */
        else {
            return compare_complex_and_complex(
                value_a, indices_a, real_a, value_b, indices_b, real_b);
        }

        break;

    case LGLSXP:
        for (int i = 0; i < length; ++i) {
            if (!is_equal_logical(
                    value_a, indices_a[i], value_b, indices_b[i])) {
                return false;
            }
        }
        return true;
        break;

    case CPLXSXP:
        for (int i = 0; i < length; ++i) {
            if (!is_equal_complex(
                    value_a, indices_a[i], value_b, indices_b[i])) {
                return false;
            }
        }
        return true;
        break;

    case RAWSXP:
        for (int i = 0; i < length; ++i) {
            if (!is_equal_raw(value_a, indices_a[i], value_b, indices_b[i])) {
                return false;
            }
        }
        return true;
        break;

    case STRSXP:
        for (int i = 0; i < length; ++i) {
            if (!is_equal_character(
                    value_a, indices_a[i], value_b, indices_b[i])) {
                return false;
            }
        }
        return true;
        break;

    default:
        Rf_error("Unknown type '%s' encountered by to_sexp",
                 type2str(sexptype_a));
    }
}

result_t parse_range_pattern(const std::string& function_name,
                             SEXP r_expression) {
    const int length = Rf_length(r_expression);

    if (length < 2 || length > 4) {
        std::string message = "range passed invalid number of arguments";
        return result::from_pattern(message);
    }

    result_t inner = parse_helper(CADR(r_expression));

    if (inner.has_error()) {
        return inner;
    }

    int min = 0;
    int max = INT_MAX;

    if (length >= 3) {
        SEXP r_min = CADDR(r_expression);

        if (TYPEOF(r_min) == INTSXP) {
            min = INTEGER(r_min)[0];
        } else if (TYPEOF(r_min) == REALSXP) {
            min = REAL(r_min)[0];
        } else {
            delete inner.get_pattern();
            std::string message = "invalid minimum for range";
            return result::from_pattern(message);
        }
    }

    if (length == 4) {
        SEXP r_max = CADDDR(r_expression);

        if (TYPEOF(r_max) == INTSXP) {
            max = INTEGER(r_max)[0];
        } else if (TYPEOF(r_max) == REALSXP) {
            max = REAL(r_max)[0];
        } else {
            delete inner.get_pattern();
            std::string message = "invalid maximum for range";
            return result::from_pattern(message);
        }
    }

    if (min < 0 || max < 0 || max < min) {
        delete inner.get_pattern();
        std::string message = "invalid minimum or maximum for range";
        return result::from_pattern(message);
    }

    return result::from_pattern(
        new RangeUnaryPattern(r_expression, inner.get_pattern(), min, max));
}
