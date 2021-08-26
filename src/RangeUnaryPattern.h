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
        get_match_interval().set_minimum(minimum);
        get_match_interval().set_maximum(maximum);
    }

    Context match(RValue value,
                  SEXP r_pat_env,
                  const Context& context) const override final {
        Context clone(context);

        Interval index_interval(value.get_index_interval());

        int minimum_index = index_interval.get_minimum();
        int middle_index = minimum_index + get_match_interval().get_minimum();
        int maximum_index = index_interval.get_maximum();

        IdentifierNames identifier_names = get_identifier_names();

        std::unordered_map<std::string, std::vector<SEXP>> matches;

        for (int index = 0; index < identifier_names.get_count(); ++index) {
            matches.insert(
                {identifier_names.get_name(index), std::vector<SEXP>()});
        }

        int match_count = 0;

        /* this range absolutely has to match */
        for (int match_index = minimum_index; match_index < middle_index;
             ++match_index) {
            value.get_index_interval().set(match_index);
            Context clone_required =
                get_sub_pattern()->match(value, r_pat_env, clone);
            /* if matching fails, return */
            if (!clone_required) {
                clone_required.set_match_count(match_count);
                return clone_required;
            }
            /* if matching succeeds, copy the bindings temporarily */
            else {
                ++match_count;
                for (auto& key_value: matches) {
                    key_value.second.push_back(clone_required.get_bindings()
                                                   .lookup(key_value.first)
                                                   .get_value());
                }
            }
        }

        /* this range does not necessarily have to match */
        for (int match_index = middle_index; match_index < maximum_index;
             ++match_index) {
            value.get_index_interval().set(match_index);
            Context clone_required =
                get_sub_pattern()->match(value, r_pat_env, clone);
            /* if matching fails, return */
            if (!clone_required) {
                break;
            }
            /* if matching succeeds, copy the bindings temporarily */
            else {
                ++match_count;
                for (auto& key_value: matches) {
                    key_value.second.push_back(clone_required.get_bindings()
                                                   .lookup(key_value.first)
                                                   .get_value());
                }
            }
        }

        for (auto& key_value: matches) {
            const std::vector<SEXP>& values = key_value.second;

            SEXP r_values = PROTECT(allocVector(VECSXP, values.size()));
            for (int index = 0; index < values.size(); ++index) {
                SET_VECTOR_ELT(r_values, index, values[index]);
            }
            UNPROTECT(1);
            clone.get_bindings().bind(key_value.first, r_values);
        }

        clone.set_success();
        clone.set_match_count(match_count);
        return clone;
    }

    IdentifierNames get_identifier_names() const override final {
        return get_sub_pattern()->get_identifier_names();
    }
};

#endif /* MATCHR_RANGE_UNARY_PATTERN_H */
