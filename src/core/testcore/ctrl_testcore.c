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
#include "ctrl_testcore.h"
#include "leafpy_cfg.h"
#include "error_code.h"
#include "log.h"
#include "mm.h"
#include "test_parser.h"
#include "test_vm.h"
#include "test_core.h"

int ctrl_testcore_init(u8 core_id)
{
    int ret;
    ret = test_vm_init(core_id, TEST_FRAME_QUEUE_0);
    if (ret != EC_OK) {
        core_log("[ctrl] test_vm init failed, ret[%d]\n", ret);
        return ret;
    }

    ret = test_core_init(core_id);
    if (ret != EC_OK) {
        core_log("[ctrl] test_core init failed, ret[%d]\n", ret);
        return ret;
    }

    return EC_OK;
}

int ctrl_testcore_run_code(u8 core_id, const char *code, u32 code_len, char *result, u32 result_len)
{
    int ret;
    ret = test_parser_parse_code(code, code_len, TEST_FRAME_QUEUE_0);
    if (ret != EC_OK) {
        core_log("[ctrl] test_parser parse failed, ret[%d]\n", ret);
        return ret;
    }
    return EC_OK;
}

int ctrl_testcore_run_bytecode(u8 core_id, const char *bc, u32 bc_len, char *result, u32 result_len)
{
    return EC_OK;
}

int ctrl_testcore_add_core(u8 core_id)
{
    int ret;
    ret = test_vm_add(core_id, TEST_FRAME_QUEUE_1);
    if (ret != EC_OK) {
        core_log("[ctrl] test_vm add failed, ret[%d]\n", ret);
        goto error_exit;
    }

    ret = test_core_add(core_id);
    if (ret != EC_OK) {
        core_log("[ctrl] test_core add failed, ret[%d]\n", ret);
        goto error_exit;
    }
    return EC_OK;

error_exit:
    if (ret == EC_ALLOC_FAILED) {
        test_vm_free(core_id);
        test_vm_free(core_id);
    }
    return ret;
}