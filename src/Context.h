#ifndef MATCHR_CONTEXT_H
#define MATCHR_CONTEXT_H

#include "Bindings.h"
#include "Range.h"
#include "utilities.h"

class Context {
  public:
    explicit Context(bool success): success_(success), bindings_() {
    }

    explicit Context(const Bindings& bindings)
        : success_(true), bindings_(bindings) {
    }

    Context merge(const Context& context) {
        if (is_failure() || context.is_failure()) {
            return Context(false);
        }

        Optional<Bindings> bindings =
            get_bindings().merge(context.get_bindings());

        if (bindings) {
            return Context(bindings.get_value());
        } else {
            return Context(false);
        }
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

    const Bindings& get_bindings() const {
        return bindings_;
    }

    Bindings& get_bindings() {
        return bindings_;
    }

  private:
    bool success_;
    Bindings bindings_;
};

#endif /* MATCHR_CONTEXT_H */
