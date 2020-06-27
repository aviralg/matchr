#ifndef MATCHR_BINDINGS_H
#define MATCHR_BINDINGS_H

#include <string>
#include <vector>
#include "r.h"
#include "equality.h"
#include "Optional.h"
#include "utilities.h"

namespace matchr {

class Bindings {
  public:
    explicit Bindings() {
    }

    int get_size() const {
        return identifiers_.size();
    }

    const std::vector<std::string>& get_identifiers() const {
        return identifiers_;
    }

    Optional<SEXP> lookup(const std::string& identifier) const {
        for (int index = 0; index < get_size(); ++index) {
            if (identifiers_[index] == identifier) {
                return Optional<SEXP>(r_values_[index]);
            }
        }

        return Optional<SEXP>();
    }

    bool bind(const std::string& identifier, SEXP r_value) {
        int index;

        for (index = 0; index < get_size(); ++index) {
            if (identifiers_[index] == identifier) {
                return false;
            }
            if (identifiers_[index] > identifier) {
                break;
            }
        }

        identifiers_.insert(identifiers_.begin() + index, identifier);
        r_values_.insert(r_values_.begin() + index, r_value);
        return true;
    }

    SEXP as_environment(SEXP parent) {
        SEXP hash = PROTECT(ScalarLogical(true));
        SEXP size = PROTECT(ScalarInteger(get_size()));

        SEXP r_environment = PROTECT(new_dot_env(hash, parent, size));

        for (int index = 0; index < get_size(); ++index) {
            SEXP r_symbol = Rf_install(identifiers_[index].c_str());
            Rf_defineVar(r_symbol, r_values_[index], r_environment);
        }

        UNPROTECT(3);

        return r_environment;
    };

    bool is_mergeable(const Bindings& bindings) const {
        const std::vector<std::string>& identifiers(bindings.get_identifiers());
        int this_index = 0;
        int this_size = get_size();

        int that_index = 0;
        int that_size = bindings.get_size();

        while (this_index < this_size && that_index < that_size) {
            if (identifiers_[this_index] == identifiers[that_index]) {
                SEXP this_value = lookup(identifiers_[this_index]).get_value();
                SEXP that_value =
                    bindings.lookup(identifiers_[that_index]).get_value();
                if (!is_equal(this_value, that_value)) {
                    return false;
                }
                ++this_index;
                ++that_index;
            }

            else if (identifiers_[this_index] < identifiers[that_index]) {
                ++this_index;
            }

            else {
                ++that_index;
            }
        }
        return true;
    }

  private:
    std::vector<std::string> identifiers_;
    std::vector<SEXP> r_values_;
};

} // namespace matchr

#endif /* MATCHR_BINDINGS_H */
