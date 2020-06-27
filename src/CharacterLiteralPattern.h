#ifndef MATCHR_CHARACTER_LITERAL_PATTERN_H
#define MATCHR_CHARACTER_LITERAL_PATTERN_H

#include "LiteralPattern.h"

namespace matchr {

class CharacterLiteralPattern: public LiteralPattern {
  public:
    CharacterLiteralPattern(SEXP r_expression,
                            SEXP r_environment,
                            const std::string& value)
        : LiteralPattern(r_expression, r_environment), value_(value) {
    }

    const std::string& get_value() const {
        return value_;
    }

    Context match_value(RValue value,
                        const Context& context) const override final {
        Context clone(context);

        bool status = value.is_character_vector() &&
                      value.has_character_value(get_value());

        clone.set_status(status);

        return clone;
    }

  private:
    const std::string value_;
};

} // namespace matchr

#endif /* MATCHR_CHARACTER_LITERAL_PATTERN_H */
