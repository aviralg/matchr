#include "Pattern.h"
#include "IdentifierPattern.h"
#include "WildcardPattern.h"
#include "AndSequencePattern.h"
#include "OrSequencePattern.h"

namespace matchr {

SEXP Pattern::class_ = NULL;

void Pattern::initialize() {
    class_ = Object::create_class("matchr_pattern");
    R_PreserveObject(class_);
}

void Pattern::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Pattern::get_class() {
    return class_;
}

PatternSPtr Pattern::from_sexp(SEXP r_pattern) {
    void* pattern = R_ExternalPtrAddr(r_pattern);
    if (pattern == NULL) {
        Rf_errorcall(R_NilValue, "Pattern::from_sexp: object is null");
    } else {
        return *static_cast<PatternSPtr*>(pattern);
    }
}

SEXP Pattern::to_sexp(PatternSPtr pattern) {
    SEXP r_pattern = PROTECT(
        R_MakeExternalPtr(new PatternSPtr(pattern), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_pattern, Pattern::destroy_sexp, TRUE);

    setAttrib(r_pattern, R_ClassSymbol, Pattern::get_class());

    UNPROTECT(1);

    return r_pattern;
}

void Pattern::destroy_sexp(SEXP r_pattern) {
    delete static_cast<PatternSPtr*>(R_ExternalPtrAddr(r_pattern));
    R_SetExternalPtrAddr(r_pattern, NULL);
}

Pattern* Pattern::create(SEXP expression) {
    Pattern* pattern = nullptr;

    if (pattern = WildcardPattern::create(expression)) {
        return pattern;
    }
    if (pattern = IdentifierPattern::create(expression)) {
        return pattern;
    }
    if (pattern = AndSequencePattern::create(expression)) {
        return pattern;
    }
    if (pattern = OrSequencePattern::create(expression)) {
        return pattern;
    }

    return pattern;
}

} // namespace matchr
