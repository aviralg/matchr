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

    Context match(input_t input, SEXP r_pat_env) const override final {
        int length = value::get_length(value);

        std::vector<Context> contexts;
        contexts.reserve(length);

        const Pattern* pattern = get_sub_pattern();

        for (int i = 0; i < length; ++i) {
            value_t new_value = value::extract(value, i);
            Context result = pattern->match(new_value, r_pat_env);

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
        std::vector<std::vector<output_t>> outputs;

        for (int i = 0; i < contexts.size(); ++i) {
            const Bindings& bindings = contexts[i].get_bindings();

            int a = 0;
            int size_a = identifiers.size();

            int b = 0;
            int size_b = bindings.get_size();

            while (a < size_a && b < size_b) {
                const std::string& identifier = identifiers[a];
                const Bindings::Cell& cell_b = bindings.get_cell(b);

                if (identifier == cell_b.identifier) {
                    outputs[a].push_back(cell_b.output);
                    ++a;
                    ++b;
                }

                else if (identifier < cell_b.identifier) {
                    ++a;
                }

                else {
                    identifiers.insert(identifiers.begin() + a,
                                       cell_b.identifier);
                    outputs.insert(outputs.begin() + a, {cell_b.output});
                    ++a;
                    ++size_a;
                    ++b;
                }
            }

            while (b < size_b) {
                const Bindings::Cell& cell_b = bindings.get_cell(b);

                identifiers.push_back(cell_b.identifier);
                outputs.push_back({cell_b.output});
                ++b;
            }
        }

        std::vector<Bindings::Cell> cells;
        cells.reserve(identifiers.size());
        for (int i = 0; i < identifiers.size(); ++i) {
            Bindings::Cell cell{.identifier = identifiers[i],
                                .output =
                                    output::from_components(type, outputs[i])};

            cells.push_back(cell);
        }

        return Context(Bindings(cells));
    }
};

#endif /* MATCHR_RANGE_UNARY_PATTERN_H */
