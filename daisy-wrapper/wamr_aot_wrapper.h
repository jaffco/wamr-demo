#pragma once
#include <wasm_export.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    wasm_module_t module;
    wasm_module_inst_t instance;
    wasm_exec_env_t exec_env;
    wasm_function_inst_t process_func;
} WamrAotEngine;

WamrAotEngine* wamr_aot_engine_new(void);
void wamr_aot_engine_delete(WamrAotEngine* engine);
bool wamr_aot_engine_load_embedded_module(WamrAotEngine* engine);
void wamr_aot_engine_process(WamrAotEngine* engine, const float* input, float* output, int num_samples);

#ifdef __cplusplus
}
#endif
