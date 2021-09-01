#ifndef MATCHR_BINDINGS_H
#define MATCHR_BINDINGS_H

#include <string>
#include <vector>
#include "r.h"
#include "equality.h"
#include "Optional.h"
#include "utilities.h"
#include "RValue.h"
#include <iostream>
class Bindings {
  public:
    struct Cell {
        std::string identifier;
        RValue r_value;
    };

    Bindings() {
    }

    Bindings(const std::vector<Cell>& cells): Bindings() {
        cells_ = cells;
    }

    int get_size() const {
        return cells_.size();
    }

    const Cell& get_cell(int index) const {
        return cells_[index];
    }

    void bind(const std::string& identifier, RValue r_value) {
        int index;

        for (index = 0; index < get_size(); ++index) {
            const Cell& cell = get_cell(index);

            if (cell.identifier == identifier) {
                Rf_error("unexpected duplicate bind encountered");
            }

            if (cell.identifier > identifier) {
                break;
            }
        }

        Cell cell = {.identifier = identifier, .r_value = r_value};
        cells_.insert(cells_.begin() + index, cell);
    }

    SEXP as_environment(SEXP r_parent) const {
        SEXP r_environment = PROTECT(new_dot_env(true, get_size(), r_parent));

        std::cout << "here begin" << std::endl;

        for (int index = 0; index < get_size(); ++index) {
            const Cell& cell = get_cell(index);
            std::cout << cell.identifier << std::endl;
            SEXP r_symbol = PROTECT(Rf_install(cell.identifier.c_str()));
            SEXP r_value = PROTECT(cell.r_value.to_sexp());
            Rf_defineVar(r_symbol, r_value, r_environment);
            UNPROTECT(2);
        }

        std::cout << "here end" << std::endl;

        UNPROTECT(1);

        return r_environment;
    };

    /* cells are sorted by identifier, so merging is merge sort's merge step
       which is a linear time algorithm */
    Optional<Bindings> merge(const Bindings& bindings) const {
        std::vector<std::string> identifiers;
        std::vector<RValue> r_value;
        std::vector<Cell> cells;

        int a = 0;
        int b = 0;
        int size_a = get_size();
        int size_b = bindings.get_size();

        while (a < size_a && b < size_b) {
            const Cell& cell_a = get_cell(a);
            const Cell& cell_b = bindings.get_cell(b);

            if (cell_a.identifier == cell_b.identifier) {
                if (cell_a.r_value != cell_b.r_value) {
                    return {};
                }

                cells.push_back(cell_a);

                ++a;
                ++b;
            }

            else if (cell_a.identifier < cell_b.identifier) {
                cells.push_back(cell_a);
                ++a;
            }

            else {
                cells.push_back(cell_b);
                ++b;
            }
        }

        while (a < size_a) {
            const Cell& cell_a = get_cell(a);
            cells.push_back(cell_a);
            ++a;
        }

        while (b < size_b) {
            const Cell& cell_b = bindings.get_cell(b);
            cells.push_back(cell_b);
            ++b;
        }

        return {Bindings(cells)};
    }

    // RValue lookup(const std::string& identifier) const {
    //     for (int index = 0; index < get_size(); ++index) {
    //         if (identifiers_[index] == identifier) {
    //             return Optional<RValue>(r_values_[index]);
    //         }
    //     }
    //
    //     return Optional<RValue>();
    // }

    // const std::vector<std::string>& get_identifiers() const {
    //    return identifiers_;
    //}

    // bool is_mergeable(const Bindings& bindings) const {
    //    const std::vector<std::string>&
    //    identifiers(bindings.get_identifiers()); int this_index = 0; int
    //    this_size = get_size();
    //
    //    int that_index = 0;
    //    int that_size = bindings.get_size();
    //
    //    while (this_index < this_size && that_index < that_size) {
    //        if (identifiers_[this_index] == identifiers[that_index]) {
    //            SEXP this_value =
    //                lookup(identifiers_[this_index]).get_value().to_sexp();
    //            SEXP that_value =
    //            bindings.lookup(identifiers_[that_index])
    //                                  .get_value()
    //                                  .to_sexp();
    //            if (!is_equal(this_value, that_value)) {
    //                return false;
    //            }
    //            ++this_index;
    //            ++that_index;
    //        }
    //
    //        else if (identifiers_[this_index] < identifiers[that_index]) {
    //            ++this_index;
    //        }
    //
    //        else {
    //            ++that_index;
    //        }
    //    }
    //    return true;
    //}

  private:
    /* identifier and rvalue are separate vectors to retrieve identifier
       vector in constant time. */
    std::vector<Cell> cells_;
};

#endif /* MATCHR_BINDINGS_H */
