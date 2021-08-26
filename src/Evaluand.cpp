#include "Evaluand.h"

Evaluand* Evaluand::create(SEXP r_expression) {
    return new Evaluand(r_expression);
}
