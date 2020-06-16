#include "r_evaluand.h"
#include "Evaluand.h"

using matchr::Evaluand;
using matchr::EvaluandSPtr;

SEXP r_evaluand_create_evaluand(SEXP r_expression, SEXP r_environment) {
    EvaluandSPtr evaluand =
        std::make_shared<Evaluand>(r_expression, r_environment);
    return Evaluand::to_sexp(evaluand);
}

SEXP r_evaluand_get_expression(SEXP r_evaluand) {
    EvaluandSPtr evaluand = Evaluand::from_sexp(r_evaluand);
    return evaluand->get_expression();
}

SEXP r_evaluand_get_environment(SEXP r_evaluand) {
    EvaluandSPtr evaluand = Evaluand::from_sexp(r_evaluand);
    return evaluand->get_environment();
}
