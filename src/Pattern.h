#ifndef MATCHR_PATTERN_H
#define MATCHR_PATTERN_H

#include "Object.h"

namespace matchr {
class Pattern: public Object {
  public:
    explicit Pattern(SEXP r_expression): Object(), r_expression_(r_expression) {
        R_PreserveObject(r_expression_);
    }

    virtual ~Pattern() {
        R_ReleaseObject(r_expression_);
    }

    SEXP get_expression() {
        return r_expression_;
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Pattern> from_sexp(SEXP r_pattern);

    static SEXP to_sexp(std::shared_ptr<Pattern> pattern);

    static void destroy_sexp(SEXP r_pattern);

  private:
    SEXP r_expression_;

    static SEXP class_;
};

using PatternSPtr = std::shared_ptr<Pattern>;

} // namespace matchr

#endif /* MATCHR_PATTERN_H */