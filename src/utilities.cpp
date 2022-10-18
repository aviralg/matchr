#include "utilities.h"

SEXP NewDotEnvSymbol = NULL;
SEXP PackageEnvironment = NULL;

void initialize_utilities(SEXP r_package_environment) {
    NewDotEnvSymbol = Rf_install("new.env");

    PackageEnvironment = r_package_environment;
}

SEXP new_environment(bool hash, int size, SEXP r_parent) {
    SEXP r_hash = PROTECT(ScalarLogical(hash));
    SEXP r_size = PROTECT(ScalarInteger(size));

    SEXP r_env = Rf_eval(Rf_lang4(NewDotEnvSymbol, r_hash, r_parent, r_size),
                         PackageEnvironment);

    UNPROTECT(2);

    return r_env;
}

struct eval_data_t {
    SEXP r_expr;
    SEXP r_env;
};

SEXP exec_eval_data(void* data) {
    eval_data_t* eval_data = (eval_data_t*) (data);
    return Rf_eval(eval_data->r_expr, eval_data->r_env);
}

struct cleanup_data_t {
    input_t input;
    output_t output;
    int unprotect_count;
};

void cleanfun(void* data) {
    cleanup_data_t* cleanup_data = (cleanup_data_t*) (data);
    input::destroy(cleanup_data->input, true);
    output::destroy(cleanup_data->output, true);
    UNPROTECT(cleanup_data->unprotect_count);
}

SEXP safe_eval(SEXP r_expr,
               SEXP r_env,
               input_t input,
               output_t output,
               int unprotect_count) {
    eval_data_t eval_data{.r_expr = r_expr, .r_env = r_env};

    cleanup_data_t cleanup_data{
        .input = input, .output = output, .unprotect_count = unprotect_count};

    R_ExecWithCleanup(exec_eval_data, &eval_data, cleanfun, &cleanup_data)
}
