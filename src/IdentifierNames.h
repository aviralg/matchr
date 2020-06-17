#ifndef MATCHR_IDENTIFIER_NAMES_H
#define MATCHR_IDENTIFIER_NAMES_H

#include <vector>
#include <string>

namespace matchr {

class IdentifierNames {
  public:
    IdentifierNames() {
    }

    IdentifierNames(const std::string& name): IdentifierNames() {
        names_.push_back(name);
    }

    bool operator==(const IdentifierNames& that) const {
        if (that.names_.size() != names_.size()) {
            return false;
        }

        for (int i = 0; i < names_.size(); ++i) {
            if (names_[i] != that.names_[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const IdentifierNames& that) const {
        return !operator==(that);
    }

    IdentifierNames& operator+=(const IdentifierNames& that) {
        merge(that);
        return *this;
    }

    IdentifierNames& operator=(const IdentifierNames& that) {
        names_ = that.names_;
        return *this;
    }

    int get_count() const {
        return names_.size();
    }

    const std::string& get_name(int index) const {
        return names_.at(index);
    }

    void add(const std::string& name) {
        int count = get_count();
        int index;

        for (index = 0; index < names_.size(); ++index) {
            const std::string& current_name = names_[index];
            if (name == current_name) {
                return;
            } else if (name > current_name) {
                break;
            }
        }

        names_.push_back("");

        for (int position = count - 1; position >= index; --position) {
            names_[position + 1] = names_[position];
        }

        names_[index] = name;
    }

    void merge(const IdentifierNames& that) {
        for (int index = 0; index < that.get_count(); ++index) {
            add(that.get_name(index));
        }
    }

  private:
    std::vector<std::string> names_;
};

} // namespace matchr

#endif /* MATCHR_IDENTIFIER_NAMES_H */
