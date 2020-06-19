#ifndef MATCHR_CONTEXT_H
#define MATCHR_CONTEXT_H

#include "Object.h"
#include "utilities.h"
#include <string>
#include <unordered_map>
#include "equality.h"
#include "Range.h"

namespace matchr {

class Context: public Object {
  public:
    explicit Context(bool success): Object(), success_(success) {
    }

    /* TODO: add move assignment and make it efficient */
    Context& operator=(const Context& context) {
        bindings_ = context.bindings_;
        success_ = context.success_;
        return *this;
    }

    bool is_successful() const {
        return success_;
    }

    bool is_failure() const {
        return !is_successful();
    }

    void set_failure() {
        success_ = false;
    }

    void set_success() {
        success_ = true;
    }

    void set_status(bool status) {
        success_ = status;
    }

    void toggle_status() {
        success_ = !success_;
    }

    operator bool() const {
        return is_successful();
    }

    const Range& get_range() const {
        return range_;
    }

    Range& get_range() {
        return range_;
    }

    void bind(const std::string& identifier, SEXP r_expression) {
        bindings_[identifier] = r_expression;
    }

    SEXP lookup(const std::string identifier) {
        auto iter = bindings_.find(identifier);

        if (iter == bindings_.end()) {
            Rf_errorcall(
                R_NilValue, "%s not bound in context", identifier.c_str());
        }

        return iter->second;
    }

    SEXP as_environment(SEXP parent) {
        SEXP hash = PROTECT(ScalarLogical(true));
        SEXP size = PROTECT(ScalarInteger(bindings_.size()));

        SEXP environment = PROTECT(new_dot_env(hash, parent, size));

        for (auto& binding: bindings_) {
            SEXP symbol = Rf_install(binding.first.c_str());
            Rf_defineVar(symbol, binding.second, environment);
        }

        UNPROTECT(3);

        return environment;
    };

  private:
    std::unordered_map<std::string, SEXP> bindings_;
    bool success_;
    Range range_;
};

} // namespace matchr

#endif /* MATCHR_CONTEXT_H */
