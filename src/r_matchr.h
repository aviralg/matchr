#ifndef MATCHR_R_MATCHR_HPP
#define MATCHR_R_MATCHR_HPP

#include "r.h"

extern "C" {
SEXP r_matchr_initialize_matchr(SEXP r_package_environment);
SEXP r_matchr_finalize_matchr();
}

#endif /* MATCHR_R_MATCHR_H */
