/*
 * Native math function imports for WAMR AOT.
 *
 * When compiled with --wrap=<func>, all calls to <func> become calls to
 * __wrap_<func>. These declarations mark __wrap_<func> as WASM imports
 * from the "env" module, resolved by the WAMR host with native ARM code.
 *
 * This avoids Emscripten's software math implementations running as WASM.
 */

/* float versions */
__attribute__((import_module("env"), import_name("tanhf")))
float __wrap_tanhf(float);

__attribute__((import_module("env"), import_name("expf")))
float __wrap_expf(float);

__attribute__((import_module("env"), import_name("logf")))
float __wrap_logf(float);

__attribute__((import_module("env"), import_name("sinf")))
float __wrap_sinf(float);

__attribute__((import_module("env"), import_name("cosf")))
float __wrap_cosf(float);

__attribute__((import_module("env"), import_name("tanf")))
float __wrap_tanf(float);

/* double versions */
__attribute__((import_module("env"), import_name("tanh")))
double __wrap_tanh(double);

__attribute__((import_module("env"), import_name("exp")))
double __wrap_exp(double);

__attribute__((import_module("env"), import_name("log")))
double __wrap_log(double);

__attribute__((import_module("env"), import_name("sin")))
double __wrap_sin(double);

__attribute__((import_module("env"), import_name("cos")))
double __wrap_cos(double);

__attribute__((import_module("env"), import_name("tan")))
double __wrap_tan(double);
