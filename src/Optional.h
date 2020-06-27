#ifndef MATCHR_OPTIONAL_H
#define MATCHR_OPTIONAL_H

namespace matchr {

template <typename T>
class Optional {
  public:
    Optional(): status_(false) {
    }

    Optional(T value): status_(true), value_(value) {
    }

    operator bool() const {
        return has_value();
    }

    bool has_value() const {
        return status_;
    }

    T get_value() const {
        return value_;
    }

  private:
    bool status_;
    T value_;
};

} // namespace matchr

#endif /* MATCHR_OPTIONAL_H */
