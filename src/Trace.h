#ifndef MATCHR_TRACE_H
#define MATCHR_TRACE_H

#include "Rincludes.h"
#include <iostream>
#include "utils.h"
#include <string>
#include "Context.h"

class Trace {
  public:
    Trace(bool enable): enable_(enable), indent_(0) {
    }

    void enter_pattern(pattern_t pattern) {
        if (!enable_)
            return;

        std::string spaces(4 * indent_, ' ');

        std::cerr << spaces;

        SEXP r_vec = PROTECT(deparse(pattern_expr(pattern)));

        std::cerr << "Match: " << CHAR(STRING_ELT(r_vec, 0)) << std::endl;

        for (int i = 1; i < Rf_length(r_vec); ++i) {
            std::cerr << spaces << CHAR(STRING_ELT(r_vec, i)) << std::endl;
        }

        UNPROTECT(1);

        ++indent_;
    }

    void exit_pattern(Context* context) {
        if (!enable_)
            return;

        --indent_;

        std::string spaces(4 * indent_, ' ');

        std::cerr << spaces;

        std::cerr << "Result: " << context -> is_true() << std::endl;
    }

  private:
    bool enable_;
    int indent_;
};

#endif /* MATCHR_TRACE_H */
