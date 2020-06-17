#ifndef MATCHR_PATTERN_H
#define MATCHR_PATTERN_H

#include "Object.h"
#include "Context.h"
#include "IdentifierNames.h"
#include "Range.h"

namespace matchr {

class Pattern: public Object {
  public:
    explicit Pattern(SEXP r_expression)
        : Object()
        , r_expression_(r_expression)
        , identifier_names_()
        , range_(1, 1) {
        R_PreserveObject(r_expression_);
    }

    virtual ~Pattern() {
        R_ReleaseObject(r_expression_);
    }

    SEXP get_expression() {
        return r_expression_;
    }

    virtual Context match_expression(SEXP expression) const {
        Context context(true);
        return match_expression(expression, context);
    }

    virtual Context& match_expression(SEXP expression,
                                      Context& context) const = 0;

    const IdentifierNames& get_identifier_names() const {
        return identifier_names_;
    }

    IdentifierNames& get_identifier_names() {
        return identifier_names_;
    }

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

    static Pattern* create(SEXP expression);

  private:
    SEXP r_expression_;
    IdentifierNames identifier_names_;
    Range range_;

    static SEXP class_;
};

using PatternSPtr = std::shared_ptr<Pattern>;

} // namespace matchr

#endif /* MATCHR_PATTERN_H */
