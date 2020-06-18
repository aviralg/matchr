#ifndef MATCHR_PATTERN_H
#define MATCHR_PATTERN_H

#include "Object.h"
#include "Context.h"
#include "IdentifierNames.h"
#include "Range.h"

namespace matchr {

class Pattern: public Object {
  public:
    explicit Pattern(SEXP r_expression, SEXP r_environment)
        : Object()
        , r_expression_(r_expression)
        , r_environment_(r_environment)
        , range_(1, 1) {
        R_PreserveObject(r_expression_);
        R_PreserveObject(r_environment_);
    }

    virtual ~Pattern() {
        R_ReleaseObject(r_expression_);
        R_ReleaseObject(r_environment_);
    }

    SEXP get_expression() const {
        return r_expression_;
    }

    SEXP get_environment() const {
        return r_environment_;
    }

    virtual Context match_value(SEXP r_value) const {
        Context context(true);
        return match_value(r_value, context);
    }

    virtual Context& match_value(SEXP r_value, Context& context) const = 0;

    virtual IdentifierNames get_identifier_names() const = 0;

    const Range& get_range() const {
        return range_;
    }

    Range& get_range() {
        return range_;
    }

    static void initialize();

    static void finalize();

    static SEXP get_class();

    static std::shared_ptr<Pattern> from_sexp(SEXP r_pattern);

    static SEXP to_sexp(std::shared_ptr<Pattern> pattern);

    static void destroy_sexp(SEXP r_pattern);

  private:
    SEXP r_expression_;
    SEXP r_environment_;
    Range range_;

    static SEXP class_;
};

using PatternSPtr = std::shared_ptr<Pattern>;

} // namespace matchr

#endif /* MATCHR_PATTERN_H */
