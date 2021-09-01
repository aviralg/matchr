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

    Context match(RValue value, SEXP r_pat_env) const override final {
        bool status = value.is_character_vector() &&
                      value.get_character_element(0) == get_value();

        return Context(status);
    }

  private:
    const std::string value_;
};

#endif /* MATCHR_CHARACTER_LITERAL_PATTERN_H */
