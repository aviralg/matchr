#include "Clause.h"
#include "Pattern.h"
#include "Evaluand.h"

Clause* Clause::create(SEXP r_expression) {
    if (TYPEOF(r_expression) != LANGSXP) {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    if (Rf_length(r_expression) != 3) {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    SEXP r_head = CAR(r_expression);

    if (TYPEOF(r_head) != SYMSXP) {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    std::string head = CHAR(PRINTNAME(r_head));

    if (head != "~") {
        /* TODO: improve error message: convert r_expression to string */
        Rf_error("expected clause expression of the form '<pattern> ~ "
                 "<expression>', got: ");
    }

    return new Clause(Pattern::create(CADR(r_expression)),
                      Evaluand::create(CADDR(r_expression)));
}
