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
#include "leafpy_api.h"
#include "type.h"
#include "log.h"
#include "controller.h"
#include <stdarg.h>

/*****************************************************************************
 *                                   Init
 *****************************************************************************/
int leafpy_init(void)
{
    int ret;
    ret = controller_init();
    if (ret != 0) {
        core_log("[service] controller init failed, ret[%d]\n", ret);
    }
    return ret;
}

int leafpy_deinit(void)
{
    return 0;
}

/*****************************************************************************
 *                                   Run
 *****************************************************************************/
int leafpy_run_code(const char *code, u32 code_len, char *result, u32 result_len)
{
    int ret;
    ret = controller_run_code(code, code_len, result, result_len);
    if (ret != 0) {
        core_log("[service] controller run code failed, ret[%d]\n", ret);
    }
    return ret;
}

int leafpy_run_bytecode(const char *bytecode, u32 bytecode_len, char *result, u32 result_len)
{
    int ret;
    ret = controller_run_bytecode(bytecode, bytecode_len, result, result_len);
    if (ret != 0) {
        core_log("[service] controller run bc failed, ret[%d]\n", ret);
    }
    return ret;
}

int leafpy_run_file(const char *filepath, enum leafpy_file_type type, char *result, int result_len)
{
    return 0;
}

/*****************************************************************************
 *                                  Compile
 *****************************************************************************/
int leafpy_compile_bytecode(const char *code, int code_len, char *bytecode, int bytecode_len)
{
    return 0;
}

/*****************************************************************************
 *                                 Muti core
 *****************************************************************************/
int leafpy_add_core(const char *core_name, u32 core_id)
{
    return controller_add_core(core_id);
}

int leafpy_deinit_core(u32 core_id)
{
    return 0;
}

int leafpy_remove_core(u32 core_id)
{
    return 0;
}

int leafpy_get_core_id(const char *core_name)
{
    return 0;
}

u32 leafpy_get_core_num(void)
{
    return 1;
}

/*****************************************************************************
 *                                   Debug
 *****************************************************************************/
void leafpy_set_debug_on(void)
{
}

void leafpy_set_debug_off(void)
{
}

void leafpy_print_core_info(u32 core_id)
{
}

void leafpy_print_vm_info(void)
{
}

void leafpy_print_parsed_bytecode(const char *bytecode, int bytecode_len)
{
}