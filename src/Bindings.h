#ifndef MATCHR_BINDINGS_H
#define MATCHR_BINDINGS_H

#include <string>
#include <vector>
#include "r.h"
#include "equality.h"
#include "Optional.h"
#include "utilities.h"
#include "output.h"
#include <iostream>

class Bindings {
  public:
    struct Cell {
        std::string identifier;
        output_t output;
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

    void bind(const std::string& identifier, const output_t& output) {
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

        Cell cell = {.identifier = identifier, .output = output};
        cells_.insert(cells_.begin() + index, cell);
    }

    SEXP as_environment(SEXP r_parent) const {
        SEXP r_environment = PROTECT(new_environment(true, get_size(), r_parent));

        for (int index = 0; index < get_size(); ++index) {
            const Cell& cell = get_cell(index);
            std::cout << cell.identifier << std::endl;
            SEXP r_symbol = PROTECT(Rf_install(cell.identifier.c_str()));
            SEXP r_value = PROTECT(output::to_sexp(cell.output));
            Rf_defineVar(r_symbol, r_value, r_environment);
            UNPROTECT(2);
        }

        UNPROTECT(1);

        return r_environment;
    };

    /* cells are sorted by identifier, so merging is merge sort's merge step
       which is a linear time algorithm */
    Optional<Bindings> merge(const Bindings& bindings) const {
        std::vector<std::string> identifiers;
        std::vector<output_t> outputs;
        std::vector<Cell> cells;

        int a = 0;
        int b = 0;
        int size_a = get_size();
        int size_b = bindings.get_size();

        while (a < size_a && b < size_b) {
            const Cell& cell_a = get_cell(a);
            const Cell& cell_b = bindings.get_cell(b);

            if (cell_a.identifier == cell_b.identifier) {
                Rf_error("cannot use the same identifier name more than "
                         "once; non-linear patterns not supported yet");

                /* TODO: compare here for equality
                if (!view::is_equal(cell_a.view, cell_b.view)) {
                    return {};
                }

                cells.push_back(cell_a);

                ++a;
                ++b;
                */
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

  private:
    std::vector<Cell> cells_;
};

#endif /* MATCHR_BINDINGS_H */
