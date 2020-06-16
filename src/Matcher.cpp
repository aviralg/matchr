#include "Matcher.h"

namespace matchr {

SEXP Matcher::class_ = NULL;

void Matcher::initialize() {
    class_ = Object::create_class("matchr_matcher");
    R_PreserveObject(class_);
}

void Matcher::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Matcher::get_class() {
    return class_;
}

MatcherSPtr Matcher::from_sexp(SEXP r_matcher) {
    void* matcher = R_ExternalPtrAddr(r_matcher);
    if (matcher == NULL) {
        Rf_errorcall(R_NilValue, "Matcher::from_sexp: object is null");
    } else {
        return *static_cast<MatcherSPtr*>(matcher);
    }
}

SEXP Matcher::to_sexp(MatcherSPtr matcher) {
    SEXP r_matcher = PROTECT(
        R_MakeExternalPtr(new MatcherSPtr(matcher), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_matcher, Matcher::destroy_sexp, TRUE);

    setAttrib(r_matcher, R_ClassSymbol, Matcher::get_class());

    UNPROTECT(1);

    return r_matcher;
}

void Matcher::destroy_sexp(SEXP r_matcher) {
    delete static_cast<MatcherSPtr*>(R_ExternalPtrAddr(r_matcher));
    R_SetExternalPtrAddr(r_matcher, NULL);
}

} // namespace matchr
