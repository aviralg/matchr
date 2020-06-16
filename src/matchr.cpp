#include "Matcher.h"
#include "Clause.h"
#include "Pattern.h"
#include "Evaluand.h"
#include <vector>

namespace matchr {

bool initialized = false;

void initialize_matchr() {
    if (!initialized) {
        initialized = true;
        Matcher::initialize();
        Clause::initialize();
        Pattern::initialize();
        Evaluand::initialize();
    }
}

void finalize_matchr() {
    if (initialized) {
        initialized = false;
        Matcher::finalize();
        Clause::finalize();
        Pattern::finalize();
        Evaluand::finalize();
    }
}

} // namespace matchr
