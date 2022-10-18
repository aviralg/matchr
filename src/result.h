#ifndef MATCHR_RESULT_H
#define MATCHR_RESULT_H

struct result_t {
    void* value;
    std::string message;
};

namespace result {

result_t from_value(void* value) {
    result_t result;
    result.value = value;
    return result;
}

result_t from_message(const std::string& message) {
    result_t result;
    result.value = nullptr;
    result.message = message;
    return result;
}

inline bool has_value(const result_t result) {
    return result->value != nullptr;
}

inline bool has_message(const result_t result) {
    return !has_pattern(result);
}

inline const std::string& get_message(const result_t result) {
    return result->message;
}

inline const pattern_t get_pattern(const result_t result) {
    return static_cast<pattern_t>(result->value);
}

inline const pattern_t get_sexp(const result_t result) {
    return static_cast<SEXP>(result->value);
}

} // namespace result

#endif /* MATCHR_RESULT_H */
