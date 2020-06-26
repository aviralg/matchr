#ifndef MATCHR_MATCHR_H
#define MATCHR_MATCHR_H

#include "r.h"

namespace matchr {

void initialize_matchr(SEXP r_package_environment);

void finalize_matchr();

}; // namespace matchr

#endif /* MATCHR_MATCHR_H */
