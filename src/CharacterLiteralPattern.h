#ifndef MATCHR_CHARACTER_LITERAL_PATTERN_H
#define MATCHR_CHARACTER_LITERAL_PATTERN_H

#include "LiteralPattern.h"
#include <cstring>
#include <cstdlib>

class CharacterLiteralPattern: public LiteralPattern {
  public:
    CharacterLiteralPattern(SEXP r_expression, const char* value)
        : LiteralPattern(r_expression), value_(NULL) {
        if (value != NULL) {
            value_ = (char*) std::malloc(sizeof(char) * strlen(value));
            std::strcpy(value_, value);
        }
    }

    ~CharacterLiteralPattern() override {
        std::free(value_);
    }

    const char* get_value() const {
        return value_;
    }

    Context match(RValue value, SEXP r_pat_env) const override final {
        bool status = value.is_character_scalar();

        if (status) {
            const char* elt = value.get_character_element(0);
            if (value_ == NULL) {
                status = elt == NULL;
            } else if (elt == NULL) {
                status = false;
            } else {
                status = !strcmp(elt, value_);
            }
        }

        return Context(status);
    }

  private:
    char* value_;
};

#endif /* MATCHR_CHARACTER_LITERAL_PATTERN_H */
