#ifndef MATCHR_RANGE_UNARY_PATTERN_H
#define MATCHR_RANGE_UNARY_PATTERN_H

#include "Pattern.h"
#include "Context.h"
#include <unordered_map>

class RangeUnaryPattern: public UnaryPattern {
  public:
    explicit RangeUnaryPattern(SEXP r_expression,
                               Pattern* sub_pattern,
                               int minimum,
                               int maximum)
        : UnaryPattern(r_expression, sub_pattern) {
        set_range(Range(minimum, maximum));
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        int length = value.get_length();

        std::vector<Context> contexts;
        contexts.reserve(length);

        const Pattern* pattern = get_sub_pattern();

        for (int i = 0; i < length; ++i) {
            RValue new_value = value.extract(i);
            Context result = pattern->match(new_value, r_pat_env, context);

            if (!result) {
                return result;
            }

            contexts.push_back(result);
        }

        return merge(contexts, value.get_type());
    }

  private:
    Context merge(const std::vector<Context>& contexts, SEXPTYPE type) const {
        std::vector<std::string> identifiers;
        std::vector<Bindings::Cell> cells;
        std::vector<std::vector<RValue>> r_values_vec;

        for (int i = 0; i < contexts.size(); ++i) {
            const Bindings& bindings = contexts[i].get_bindings();

            for (int j = 0; j < bindings.get_size(); ++j) {
                Bindings::Cell cell = bindings.get_cell(j);

                for (int k = 0; k < identifiers.size(); ++k) {
                    if (identifiers[k] == cell.identifier) {
                        r_values_vec[k].push_back(cell.r_value);
                        break;
                    } else if (identifiers[k] > cell.identifier) {
                        identifiers.insert(identifiers.begin() + k,
                                           cell.identifier);
                        r_values_vec.insert(r_values_vec.begin() + k,
                                            {cell.r_value});
                        break;
                    }
                }
            }
        }

        cells.reserve(identifiers.size());
        for (int i = 0; i < identifiers.size(); ++i) {
            Bindings::Cell cell{.identifier = identifiers[i],
                                .r_value = RValue(type, r_values_vec[i])};

            cells.push_back(cell);
        }

        return Context(true, Bindings(cells));
    }
};

#endif /* MATCHR_RANGE_UNARY_PATTERN_H */
