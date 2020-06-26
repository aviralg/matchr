#include <string>
#include "r_utilities.h"

SEXP r_utilities_parse_identifier(SEXP r_identifier) {
    std::string name;
    std::string minimum;
    std::string maximum;
    std::string type;

    std::string identifier(CHAR(asChar(r_identifier)));

    int size = identifier.size();

    int range_separation_index = -1;
    int identifier_separation_index = -1;

    for (int i = size - 1; i >= 1; --i) {
        if (identifier[i] == '.') {
            range_separation_index = i;
        }
        if (identifier[i] == '_' && identifier[i - 1] == '_') {
            identifier_separation_index = i - 1;
            break;
        }
    }

    /*  */
    if (identifier_separation_index == -1) {
        type = "identifier";
        identifier = identifier;
        minimum = "";
        maximum = "";
    }
    /*  */
    else {
        type = "range";
        identifier = identifier.substr(0, identifier_separation_index);

        /* there is no dot to separate minimum and maximum in the range */
        if (range_separation_index == -1) {
            minimum = identifier.substr(identifier_separation_index + 2);
            maximum = "2147483647";

        }
        /* there is a dot to separate minimum and maximum */
        else {
            minimum = identifier.substr(identifier_separation_index + 2,
                                        range_separation_index -
                                            (identifier_separation_index + 2));
            maximum = identifier.substr(range_separation_index + 1);
        }
    }

    SEXP result = PROTECT(allocVector(VECSXP, 4));
    SET_VECTOR_ELT(result, 0, mkString(type.c_str()));
    SET_VECTOR_ELT(result, 1, mkString(identifier.c_str()));
    SET_VECTOR_ELT(result, 2, mkString(minimum.c_str()));
    SET_VECTOR_ELT(result, 3, mkString(maximum.c_str()));

    SEXP tags = PROTECT(allocVector(STRSXP, 4));
    SET_STRING_ELT(tags, 0, mkChar("type"));
    SET_STRING_ELT(tags, 1, mkChar("identifier"));
    SET_STRING_ELT(tags, 2, mkChar("minimum"));
    SET_STRING_ELT(tags, 3, mkChar("maximum"));

    setAttrib(result, R_NamesSymbol, tags);

    UNPROTECT(2);

    return result;
}

// Soul of a Machine
// Structure of scientific revolutions
//
