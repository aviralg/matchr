#ifndef MATCHR_OBJECT_H
#define MATCHR_OBJECT_H

#include <string>
#include "r.h"
#include <memory>

namespace matchr {

class Object {
  public:
    Object(): id_(Object::get_next_id_()) {
    }

    virtual ~Object() {
    }

    int get_id() const {
        return id_;
    }

    static SEXP create_class(const char* subclass);

    static SEXP create_class(const std::string& subclass);

    static std::shared_ptr<Object> from_sexp(SEXP r_object);

  private:
    int id_;

    static int get_next_id_();
    static int id_counter_;
};

using ObjectSPtr = std::shared_ptr<Object>;

} // namespace matchr

#endif /* MATCHR_OBJECT_H */
