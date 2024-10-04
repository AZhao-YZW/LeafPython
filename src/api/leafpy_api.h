/*
 * MIT License
 *
 * Copyright (c) 2024 AZhao yuzhaowei2002@outlook.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef _LEAFPY_API_H_
#define _LEAFPY_API_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "register.h"

enum leafpy_file_type {
    LEAFPY_FILE_TYPE_CODE,
    LEAFPY_FILE_TYPE_BYTECODE
};

/* Init */
int leafpy_init(void);
int leafpy_deinit(void);

/* Run */
int leafpy_run_code(const char *code, unsigned int code_len, char *result, unsigned int result_len);
int leafpy_run_bytecode(const char *bytecode, unsigned int bytecode_len, char *result, unsigned int result_len);
int leafpy_run_file(const char *filepath, enum leafpy_file_type type, char *result, int result_len);

/* Compile */
int leafpy_compile_bytecode(const char *code, int code_len, char *bytecode, int bytecode_len);

/* Register C functions */
/**
 * @brief get cfunc type
 * @param type_name support: VOID, U8, U16, U32, U64, S8, S16, S32, S64, F32, F64
 * @param p_layer pointer layer, support 0 ~ 15
 */
#define CFUNC_TYPE(type_name, p_layer) ( \
    ((leafpy_cfunc_type)(CFUNC_TYPE_CODE(type_name)) << 4) | \
    ((leafpy_cfunc_type)(CFUNC_P_LAYER(p_layer))) \
)

/**
 * @brief leafpython register cfunc
 * @param module module name
 * @param pyfunc python function name
 * @param ret_type return type, get by macro CFUNC_TYPE
 * @param arg_num argument number
 * @param ... argument types, get by macro CFUNC_TYPE
 * @return 0 success, -1 failed
 */
#define LEAFPY_REGISTER_CFUNC(module, pyfunc, ret_type, arg_num, ...) \
    ({ \
        int ret = 0; \
        if (NARG(__VA_ARGS__) != arg_num) { \
            ret = -1; \
        } else { \
            ret = leafpy_register_cfunc(module, pyfunc, ret_type, arg_num, ##__VA_ARGS__); \
        } \
        ret; \
    })

/* Muti core */
int leafpy_add_core(const char *core_name, unsigned int core_id);
int leafpy_deinit_core(unsigned int core_id);
int leafpy_remove_core(unsigned int core_id);
int leafpy_get_core_id(const char *core_name);
unsigned int leafpy_get_core_num(void);

/* Log */
#define __EXTERN_LEAFPY_LOG 1
#if (__EXTERN_LEAFPY_LOG == 1)
enum log_level_e;
void log_set_level(enum log_level_e level);
void log_printf(enum log_level_e level, const char *fmt, ...);
#endif

/* Debug */
void leafpy_set_debug_on(void);
void leafpy_set_debug_off(void);
void leafpy_print_core_info(unsigned int core_id);
void leafpy_print_vm_info(void);
void leafpy_print_parsed_bytecode(const char *bytecode, int bytecode_len);

#ifdef __cplusplus
}
#endif
#endif