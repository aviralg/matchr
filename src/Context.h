#ifndef MATCHR_CONTEXT_H
#define MATCHR_CONTEXT_H

#include "Object.h"
#include "utilities.h"
#include <string>
#include <unordered_map>
#include "equality.h"

namespace matchr {

class Context: public Object {
  public:
    explicit Context(bool success): Object(), success_(success) {
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

    operator bool() const {
        return is_successful();
    }

    bool bind(const std::string identifier, SEXP expression) {
        auto iter = bindings_.find(identifier);

        /* if this identifier is already bound to an expression, then compare
         * that for equality with the argument expression  */
        if (iter != bindings_.end()) {
            SEXP bound_expression = iter->second;
            return is_equal(bound_expression, expression);
        }

        /* add binding if identifier is not already bound */
        bindings_.insert({identifier, expression});

        return true;
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
};

} // namespace matchr

#endif /* MATCHR_CONTEXT_H */
