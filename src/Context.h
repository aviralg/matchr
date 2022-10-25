#ifndef MATCHR_CONTEXT_H
#define MATCHR_CONTEXT_H

#include <map>
#include "Value.h"
#include <unordered_map>

class Context {
  public:
    explicit Context(bool state): state_(state) {
    }

    ~Context() {
        // delete all Value* in bindings.
        for (auto it = bindings_.begin(); it != bindings_.end(); ++it) {
            delete it->second;
        }
    }

    bool is_true() const {
        return state_;
    }

    bool is_false() const {
        return !is_true();
    }

    void bind(const std::string& name, Value* value) {
        Value* value2 = value->clone();
        bool bound = bind_unsafe_(name, value2);
        if (!bound) {
            delete value2;
        }
    }

    const Value* lookup(const std::string& name, Value* other) {
        auto iter = bindings_.find(name);
        return iter == bindings_.end() ? other : iter->second;
    }

    // move bindings from other to here.
    void consume(Context* other) {
        for (auto it = other->bindings_.begin();
             it != other->bindings_.end();) {
            /* if binding was unsuccessful, delete it->second */
            bool bound = bind_unsafe_(it->first, it->second);

            if (bound) {
                it = other->bindings_.erase(it);
            } else {
                it = ++it;
            }

            if (is_false()) {
                break;
            }
        }
    }

    SEXP to_env(SEXP r_enclos) const {
        SEXP r_env = PROTECT(R_NewEnv(r_enclos, 1, bindings_.size()));

        for (auto it = bindings_.begin(); it != bindings_.end(); ++it) {
            const std::string& name = it->first;
            const Value* value = it->second;

            SEXP r_val = PROTECT(value->to_sexp());

            Rf_defineVar(Rf_install(name.c_str()), r_val, r_env);

            UNPROTECT(1);
        }

        UNPROTECT(1);

        return r_env;
    }

  private:
    bool state_;
    std::unordered_map<std::string, Value*> bindings_;

    bool bind_unsafe_(const std::string& name, Value* value) {
        auto res = bindings_.insert({name, value});
        // insertion was prevented because binding already exists
        if (!res.second) {
            state_ = value->is_equal(res.first->second);
        }

        // return if binding was successful or not
        return res.second;
    }
};

#endif /* MATCHR_CONTEXT_H */
