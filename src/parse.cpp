#include "matchr.h"
#include "matcher.h"
#include <cstdio>
#include <cstdarg>

#define MSG_BUF_SIZE 5000
char MSG_BUF[MSG_BUF_SIZE];

struct state_impl_t {
    std::vector<clause_t>* clauses;
    std::vector<pattern_t>* patterns;
};

typedef state_impl_t* state_t;

state_t state_create() {
    state_t state = new state_impl_t();
    state->clauses = new std::vector<clause_t>();
    state->patterns = new std::vector<pattern_t>();
    return state;
}

void state_destroy(state_t state) {
    std::vector<clause_t>* clauses = state->clauses;
    if (clauses != nullptr) {
        for (int i = 0; i < clauses->size(); ++i) {
            clause_destroy(clauses->at(i));
        }
        delete clauses;
    }

    std::vector<pattern_t>* patterns = state->patterns;
    if (patterns != nullptr) {
        for (int i = 0; i < patterns->size(); ++i) {
            pattern_destroy(patterns->at(i));
        }
        delete patterns;
    }

    delete state;
}

void parse_error(state_t state, const char* fmt, ...) {
    state_destroy(state);

    std::va_list args;
    va_start(args, fmt);

    std::vsnprintf(MSG_BUF, MSG_BUF_SIZE, fmt, args);

    va_end(args);

    Rf_error("%s", MSG_BUF);
}

matcher_t parse_matcher(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env);
void parse_clause(state_t state, SEXP r_clause);
void parse_pattern(state_t state, SEXP r_pattern, bool range_pat);
void parse_seq_pat(state_t state,
                   pattern_type_t pat_type,
                   SEXP r_pattern,
                   bool range_pat);
void parse_id_or_range(state_t state, const char* name, bool range_pat);

SEXP r_matchr_parse(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env) {
    matcher_t matcher = parse_matcher(r_clauses, r_pat_env, r_eval_env);
    return matcher_wrap(matcher);
}

matcher_t parse_matcher(SEXP r_clauses, SEXP r_pat_env, SEXP r_eval_env) {
    if (TYPEOF(r_clauses) != VECSXP) {
        Rf_error("expected clauses to be a list of clauses");
        return NULL;
    }

    int size = LENGTH(r_clauses);

    state_t state = state_create();

    for (int index = 0; index < size; ++index) {
        SEXP r_clause = VECTOR_ELT(r_clauses, index);
        parse_clause(state, r_clause);
    }

    matcher_t matcher =
        matcher_create(*(state->clauses), r_pat_env, r_eval_env);

    state->clauses->clear();

    // TODO: check that state->patterns is empty at this point;
    state_destroy(state);

    return matcher;
}

void parse_clause(state_t state, SEXP r_clause) {
    if (TYPEOF(r_clause) != LANGSXP) {
        /* TODO: improve error message: convert r_clause to string */
        parse_error(state,
                    "1expected clause expression of the form '<pattern> ~ "
                    "<expression>'");
    }

    if (Rf_length(r_clause) != 3) {
        /* TODO: improve error message: convert r_clause to string */
        parse_error(state,
                    "2expected clause expression of the form '<pattern> ~ "
                    "<expression>'");
    }

    SEXP r_head = CAR(r_clause);

    if (TYPEOF(r_head) != SYMSXP) {
        /* TODO: improve error message: convert r_clause to string */
        parse_error(state,
                    "3expected clause expression of the form '<pattern> ~ "
                    "<expression>'");
    }

    std::string head = CHAR(PRINTNAME(r_head));

    if (head != "~") {
        /* TODO: improve error message: convert r_clause to string */
        parse_error(state,
                    "4expected clause expression of the form '<pattern> ~ "
                    "<expression>'");
    }

    parse_pattern(state, CADR(r_clause), false);

    pattern_t pattern = state->patterns->back();
    state->patterns->pop_back();

    clause_t clause = clause_create(pattern, CADDR(r_clause));
    state->clauses->push_back(clause);
}

void parse_pattern(state_t state, SEXP r_pattern, bool range_pat) {
    SEXPTYPE t = TYPEOF(r_pattern);

    switch (t) {
    case SYMSXP:
        parse_id_or_range(state, CHAR(PRINTNAME(r_pattern)), range_pat);
        return;

    case LGLSXP: {
        pattern_t pattern = pattern_create(pattern_type_t::LGLVAL);
        pattern->lgl_val = LOGICAL(r_pattern)[0];

        state->patterns->push_back(pattern);
    }

        return;

    case INTSXP: {
        pattern_t pattern = pattern_create(pattern_type_t::INTVAL);
        pattern->int_val = INTEGER(r_pattern)[0];

        state->patterns->push_back(pattern);
    }

        return;

    case REALSXP: {
        pattern_t pattern = pattern_create(pattern_type_t::DBLVAL);
        pattern->dbl_val = REAL(r_pattern)[0];

        state->patterns->push_back(pattern);
    }

        return;

    case LANGSXP: {
        SEXP r_head = CAR(r_pattern);

        if (TYPEOF(r_head) != SYMSXP) {
            parse_error(state, "1invalid pattern");
        }

        const char* head = CHAR(PRINTNAME(r_head));

        if (!strcmp(head, "any")) {
            parse_seq_pat(state, pattern_type_t::ANY, CDR(r_pattern), false);
        }

        else if (!strcmp(head, "all")) {
            parse_seq_pat(state, pattern_type_t::ALL, CDR(r_pattern), false);
        }

        else if (!strcmp(head, "none")) {
            parse_seq_pat(state, pattern_type_t::NONE, CDR(r_pattern), false);
        }

        else if (!strcmp(head, "pat")) {
            if (Rf_length(r_pattern) != 2) {
                parse_error(state, "2invalid pattern");
            }
            parse_pattern(state, CADR(r_pattern), false);
        }

        else if (!strcmp(head, "lgl")) {
            parse_seq_pat(state, pattern_type_t::LGLVEC, CDR(r_pattern), true);
        }

        else if (!strcmp(head, "int")) {
            parse_seq_pat(state, pattern_type_t::INTVEC, CDR(r_pattern), true);
        }

        else if (!strcmp(head, "dbl")) {
            parse_seq_pat(state, pattern_type_t::DBLVEC, CDR(r_pattern), true);
        }

        else {
            parse_error(state, "3invalid pattern");
        }
    }
        return;

    default:
        parse_error(state, "4invalid pattern");
        return;
    }
}

void parse_seq_pat(state_t state,
                   pattern_type_t pat_type,
                   SEXP r_pattern,
                   bool range_pat) {
    int n = 0;

    int init_size = state->patterns->size();

    for (SEXP r_ptr = r_pattern; r_ptr != R_NilValue; r_ptr = CDR(r_ptr), ++n) {
        SEXP r_elt = CAR(r_ptr);
        // both range_pat should be true and position should be odd for range
        // pattern to be valid.
        bool temp_range_pat = range_pat && (n % 2 == 1);
        parse_pattern(state, r_elt, temp_range_pat);
    }

    pattern_t pattern = pattern_create(pat_type);

    auto left = state->patterns->begin() + init_size;
    auto right = state->patterns->end();

    pattern->patterns.assign(left, right);
    state->patterns->erase(left, right);

    state->patterns->push_back(pattern);
}

void add_range_pattern(state_t state, int min, int max) {
    pattern_t prev = state->patterns->back();
    state->patterns->pop_back();

    pattern_t pattern = pattern_create(pattern_type_t::RANGE);
    pattern->min = min;
    pattern->max = max;
    pattern->patterns.push_back(prev);

    state->patterns->push_back(pattern);
}

int parse_num(const char* name, int size, int* index) {
    int pos = *index;
    int num = 0;

    while (pos < size && name[pos] >= '0' && name[pos] <= '9') {
        num = num * 10 + (name[pos] - '0');
        ++pos;
    }

    *index = pos;

    return num;
}

char* str_dup(const char* orig, int size) {
    char* dup = (char*) std::malloc(sizeof(char) * (size + 1));
    std::strncpy(dup, orig, size);
    dup[size] = '\0';
    return dup;
}

void parse_id_or_range(state_t state, const char* name, bool range_pat) {
    int size = strlen(name);

    /* does not begin with ... */
    if (size < 3 || strncmp(name, "...", 3)) {
        pattern_t pattern = pattern_create(pattern_type_t::ID);
        pattern->id = name;
        state->patterns->push_back(pattern);
        return;
    }

    /* at this point, we know that name starts with ... */

    if (!range_pat) {
        parse_error(state, "range pattern not allowed at this position");
    }

    int index = 3;
    int min = 0;
    int max = INT_MAX;

    if (index == size) {
        add_range_pattern(state, min, max);
        return;
    } else if (isdigit(name[index])) {
        min = parse_num(name, size, &index);
    }

    if (index == size) {
        add_range_pattern(state, min, max);
        return;
    } else if (name[index] == '_') {
        ++index;
    } else {
        parse_error(state, "invalid range specifier: %s", name);
    }

    if (isdigit(name[index])) {
        max = parse_num(name, size, &index);
    } else {
        parse_error(state, "invalid range specifier: %s", name);
    }

    if (index == size) {
        add_range_pattern(state, min, max);
        return;
    } else {
        parse_error(state, "invalid range specifier: %s", name);
    }
}

