#ifndef MATCHR_SEQUENCE_PATTERN_H
#define MATCHR_SEQUENCE_PATTERN_H

#include "VariadicPattern.h"
#include "Context.h"
#include <unordered_map>
#include <iostream>

class SequencePattern: public VariadicPattern {
  public:
    explicit SequencePattern(SEXP r_expression,
                             const std::vector<Pattern*>& patterns)
        : VariadicPattern(r_expression, patterns)
        , min_right_(patterns.size(), 0)
        , max_right_(patterns.size(), 0) {
        Range range(0, 0);

        for (int i = 0; i < patterns.size(); ++i) {
            range = range.add(patterns[i]->get_range());
        }

        set_range(range);

        const int length = get_size();

        int min_acc = 0;
        int max_acc = 0;

        for (int index = length - 1; index >= 0; --index) {
            const Range& range = get_pattern(index)->get_range();

            min_right_[index] = min_acc;
            min_acc = sum_(min_acc, range.get_minimum());

            max_right_[index] = max_acc;
            max_acc = sum_(max_acc, range.get_maximum());
        }

        for (int index = 0; index < length; ++index) {
            std::cout << min_right_[index] << " | ";
        }

        std::cout << std::endl;

        for (int index = 0; index < length; ++index) {
            std::cout << max_right_[index] << " | ";
        }

        std::cout << std::endl;
    }

    Context match(input_t input, SEXP r_pat_env) const override final {
        int input_size = value.get_length();
        int pattern_count = get_size();

        /* if number of elements to be matched is greater or lesser than the
           number of elements that can be matched by the pattern, then matching
           fails; return early.*/
        if (!get_range().contains(input_size)) {
            return Context(false);
        }

        struct State {
            State()
                : initialized(false)
                , range(1, 1)
                , consumed(1)
                , current_size(1)
                , context(false) {
            }

            bool initialized;
            Range range;
            int consumed;
            int current_size;
            Context context;
        };

        std::vector<State> states(pattern_count, State());

        int index = 0;

        /* index becomes -1 when all arrangements have been tried and the
           algorithm backtracks from index 0. */
        while (index >= 0 && index < pattern_count) {
            Pattern* pattern = get_pattern(index);
            bool initialized = states[index].initialized;
            int consumed = index == 0 ? 0 : states[index - 1].consumed;

            /* the current state is uninitialized either when the size of the
               immediately preceding state has been updated or when this is
               encountered for the first time. */
            if (!initialized) {
                int remaining = input_size - consumed;
                const Range& range = pattern->get_range();

                /* the minimum number of elements this pattern can match is  */
                int min = std::max(remaining - max_right_[index],
                                   range.get_minimum());
                int max = std::min(remaining - min_right_[index],
                                   range.get_maximum());

                states[index].initialized = true;
                states[index].range = Range(min, max);
            }

            int min = states[index].range.get_minimum();
            int current_size = states[index].current_size;

            /* if the state is initialized, then we try its next size.
               Otherwise, we start from the largest number of elements it can
               take. */
            int max = initialized ? current_size - 1
                                  : states[index].range.get_maximum();

            int i;
            for (i = max; i >= min; --i) {
                states[index].current_size = i;
                states[index].consumed = consumed + i;

                RValue new_value =
                    value.subset(consumed, states[index].current_size);

                Context context = pattern->match(new_value, r_pat_env);

                if (index != 0) {
                    context = context.merge(states[index - 1].context);
                }

                /* if matching succeeds, then break and try the next pattern in
                   sequence. */
                if (context) {
                    states[index].context = context;
                    ++index;
                    break;
                }
            }

            /* matching did not succeed for any split of this pattern, go back
               and explore a new split. */
            if (i < min) {
                states[index].initialized = false;
                --index;
            }
        }

        /* if matching did not succeed; return a failure context */
        if (index != pattern_count) {
            return Context(false);
        }

        for (int index = 0; index < pattern_count; ++index) {
            std::cout << states[index].current_size << "["
                      << states[index].consumed << "]"
                      << " | ";
        }

        std::cout << std::endl;

        return states[pattern_count - 1].context;
    }

  private:
    std::vector<int> min_right_;
    std::vector<int> max_right_;

    int sum_(const int a, const int b) {
        if (a == INT_MAX || b == INT_MAX) {
            return INT_MAX;
        } else {
            return a + b;
        }
    }
};

#endif /* MATCHR_SEQUENCE_PATTERN_H */
