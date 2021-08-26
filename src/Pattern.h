#ifndef MATCHR_PATTERN_H
#define MATCHR_PATTERN_H

#include "Context.h"
#include "IdentifierNames.h"
#include "Interval.h"
#include "RValue.h"

class Pattern {
  public:
    virtual ~Pattern() {
    }

    SEXP get_expression() const {
        return r_expression_;
    }

    virtual Context match(RValue value, SEXP r_pat_env) const {
        Context context(true);
        return match(value, r_pat_env, context);
    }

    virtual Context
    match(RValue value, SEXP r_pat_env, const Context& context) const = 0;

    virtual IdentifierNames get_identifier_names() const = 0;

    const Interval& get_match_interval() const {
        return match_interval_;
    }

    Interval& get_match_interval() {
        return match_interval_;
    }

    static Pattern* create(SEXP r_expression);

  protected:
    Pattern(SEXP r_expression)
        : r_expression_(r_expression), match_interval_(false, 1, 1) {
        // We don't need to preserve r_expression because the outermost match
        // expression is preserved already by matcher.
    }

  private:
    SEXP r_expression_;
    Interval match_interval_;
};

#endif /* MATCHR_PATTERN_H */
