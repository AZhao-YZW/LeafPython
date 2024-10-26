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
#include "controller.h"
#include "ctrl_leafpy.h"
#include "ctrl_testcore.h"
#include "leafpy_cfg.h"
#include "error.h"
#include "log.h"

u8 g_cur_core_type = LEAFPY_DEFAULT_CORE_TYPE;
u8 g_cur_core_id = LEAFPY_DEFAULT_CORE_ID;

int controller_init(void)
{
    switch (g_cur_core_type) {
#if (ENABLE_CORE_LEAFPY == 1)
        case CORE_TYPE_LEAFPY:
            return ctrl_leafpy_init(g_cur_core_id);
#endif
#if (ENABLE_CORE_TESTCORE == 1)
        case CORE_TYPE_TESTCORE:
            return ctrl_testcore_init(g_cur_core_id);
#endif
        default:
            return EC_INVALID_CORE_TYPE;
    }
}

int controller_run_code(const char *code, u32 code_len, char *result, u32 result_len)
{
    if (code == NULL || code_len == 0 || result == NULL) {
        return EC_PARAM_INVALID;
    }

    switch (g_cur_core_type) {
#if (ENABLE_CORE_LEAFPY == 1)
        case CORE_TYPE_LEAFPY:
            return ctrl_leafpy_run_code(g_cur_core_id, code, code_len, result, result_len);
#endif
#if (ENABLE_CORE_TESTCORE == 1)
        case CORE_TYPE_TESTCORE:
            return ctrl_testcore_run_code(g_cur_core_id, code, code_len, result, result_len);
#endif
        default:
            return EC_INVALID_CORE_TYPE;
    }
}

int controller_run_bytecode(const char *bytecode, u32 bytecode_len, char *result, u32 result_len)
{
    if (bytecode == NULL || bytecode_len == 0 || result == NULL) {
        return EC_PARAM_INVALID;
    }

    switch (g_cur_core_type) {
#if (ENABLE_CORE_LEAFPY == 1)
        case CORE_TYPE_LEAFPY:
            return ctrl_leafpy_run_bytecode(g_cur_core_id, bytecode, bytecode_len, result, result_len);
#endif
#if (ENABLE_CORE_TESTCORE == 1)
        case CORE_TYPE_TESTCORE:
            return ctrl_testcore_run_bytecode(g_cur_core_id, bytecode, bytecode_len, result, result_len);
#endif
        default:
            return EC_INVALID_CORE_TYPE;
    }
}

int controller_add_core(u8 core_id)
{
    switch (g_cur_core_type) {
#if (ENABLE_CORE_LEAFPY == 1)
        case CORE_TYPE_LEAFPY:
            return ctrl_leafpy_add_core(core_id);
#endif
#if (ENABLE_CORE_TESTCORE == 1)
        case CORE_TYPE_TESTCORE:
            return ctrl_testcore_add_core(core_id);
#endif
        default:
            return EC_INVALID_CORE_TYPE;
    }
}