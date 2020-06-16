#include "Clause.h"

namespace matchr {

SEXP Clause::class_ = NULL;

void Clause::initialize() {
    class_ = Object::create_class("matchr_clause");
    R_PreserveObject(class_);
}

void Clause::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Clause::get_class() {
    return class_;
}

ClauseSPtr Clause::from_sexp(SEXP r_clause) {
    void* clause = R_ExternalPtrAddr(r_clause);
    if (clause == NULL) {
        Rf_errorcall(R_NilValue, "Clause::from_sexp: object is null");
    } else {
        return *static_cast<ClauseSPtr*>(clause);
    }
}

SEXP Clause::to_sexp(ClauseSPtr clause) {
    SEXP r_clause = PROTECT(
        R_MakeExternalPtr(new ClauseSPtr(clause), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_clause, Clause::destroy_sexp, TRUE);

    setAttrib(r_clause, R_ClassSymbol, Clause::get_class());

    UNPROTECT(1);

    return r_clause;
}

void Clause::destroy_sexp(SEXP r_clause) {
    delete static_cast<ClauseSPtr*>(R_ExternalPtrAddr(r_clause));
    R_SetExternalPtrAddr(r_clause, NULL);
}

} // namespace matchr
