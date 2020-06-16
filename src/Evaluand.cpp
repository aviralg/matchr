#include "Evaluand.h"

namespace matchr {

SEXP Evaluand::class_ = NULL;

void Evaluand::initialize() {
    class_ = Object::create_class("matchr_evaluand");
    R_PreserveObject(class_);
}

void Evaluand::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Evaluand::get_class() {
    return class_;
}

EvaluandSPtr Evaluand::from_sexp(SEXP r_evaluand) {
    void* evaluand = R_ExternalPtrAddr(r_evaluand);
    if (evaluand == NULL) {
        Rf_errorcall(R_NilValue, "Evaluand::from_sexp: object is null");
    } else {
        return *static_cast<EvaluandSPtr*>(evaluand);
    }
}

SEXP Evaluand::to_sexp(EvaluandSPtr evaluand) {
    SEXP r_evaluand = PROTECT(
        R_MakeExternalPtr(new EvaluandSPtr(evaluand), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_evaluand, Evaluand::destroy_sexp, TRUE);

    setAttrib(r_evaluand, R_ClassSymbol, Evaluand::get_class());

    UNPROTECT(1);

    return r_evaluand;
}

void Evaluand::destroy_sexp(SEXP r_evaluand) {
    delete static_cast<EvaluandSPtr*>(R_ExternalPtrAddr(r_evaluand));
    R_SetExternalPtrAddr(r_evaluand, NULL);
}

} // namespace matchr
