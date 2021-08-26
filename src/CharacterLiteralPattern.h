#ifndef MATCHR_CHARACTER_LITERAL_PATTERN_H
#define MATCHR_CHARACTER_LITERAL_PATTERN_H

#include "LiteralPattern.h"

class CharacterLiteralPattern: public LiteralPattern {
  public:
    CharacterLiteralPattern(SEXP r_expression, const std::string& value)
        : LiteralPattern(r_expression), value_(value) {
    }

    const std::string& get_value() const {
        return value_;
    }

    Context match(RValue value,
                  SEXP r_pat_env,
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

#endif /* MATCHR_CHARACTER_LITERAL_PATTERN_H */
