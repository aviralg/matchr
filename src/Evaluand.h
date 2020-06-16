#ifndef MATCHR_EVALUAND_H
#define MATCHR_EVALUAND_H

#include "Object.h"

namespace matchr {

class Evaluand: public Object {
  public:
    Evaluand(SEXP r_expression, SEXP r_environment)
        : Object(), r_expression_(r_expression), r_environment_(r_environment) {
        R_PreserveObject(r_expression_);
        R_PreserveObject(r_environment_);
    }

    ~Evaluand() {
        R_ReleaseObject(r_expression_);
        R_ReleaseObject(r_environment_);
    }

    SEXP get_expression() {
        return r_expression_;
    }

    SEXP get_environment() {
        return r_environment_;
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Evaluand> from_sexp(SEXP r_evaluand);

    static SEXP to_sexp(std::shared_ptr<Evaluand> evaluand);

    static void destroy_sexp(SEXP r_evaluand);

  private:
    SEXP r_expression_;
    SEXP r_environment_;

    static SEXP class_;
};

using EvaluandSPtr = std::shared_ptr<Evaluand>;

} // namespace matchr

#endif /* MATCHR_EVALUAND_H */
