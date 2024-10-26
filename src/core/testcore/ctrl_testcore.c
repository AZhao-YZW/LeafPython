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
#include "error.h"
#include "log.h"
#include "mm.h"
#include "test_parser.h"
#include "test_vm.h"
#include "test_core.h"
#include "test_frame.h"

int ctrl_testcore_init(u8 core_id)
{
    int ret;
    ret = test_vm_init(core_id);
    if (ret != EC_OK) {
        core_printf("[ctrl] test_vm init failed, ret[%d]\n", ret);
        return ret;
    }

    ret = test_core_init(core_id);
    if (ret != EC_OK) {
        core_printf("[ctrl] test_core init failed, ret[%d]\n", ret);
        return ret;
    }

    return EC_OK;
}

int ctrl_testcore_run_code(u8 core_id, const char *code, u32 code_len, char *result, u32 result_len)
{
    char *line = (char *)code;
    test_frame_s *frame = NULL;
    u32 offset = 0, line_len = 0, bc_num;
    int ret;

    while (offset < code_len) {
        // code to line
        line += line_len;
        offset += line_len;
        line_len = test_parser_get_line_len(code, code_len, offset);
        // line to frame
        bc_num = test_parser_get_frame_bc_num(line, line_len);
        frame = test_frame_init(bc_num);
        if (frame == NULL) {
            core_printf("[ctrl] test_frame init failed\n");
            return EC_ALLOC_FAILED;
        }
        // run frame
        ret = test_vm_run_frame(core_id, frame);
        if (ret != EC_OK) {
            core_printf("[ctrl] test_vm run code frame failed, ret[%d]\n", ret);
            mm_free(frame->bc_list);
            mm_free(frame);
            return ret;
        }
    }
    return EC_OK;
}

int ctrl_testcore_run_bytecode(u8 core_id, const char *bytecode, u32 bytecode_len, char *result, u32 result_len)
{
    int ret;
    test_frame_s *frame = test_frame_init(1);
    if (frame == NULL) {
        core_printf("[ctrl] test_frame init failed\n");
        return EC_ALLOC_FAILED;
    }

    ret = test_parser_bc_gen_frame(bytecode, bytecode_len, frame);
    if (ret != EC_OK) {
        core_printf("[ctrl] test_parser bc gen frame failed, ret[%d]\n", ret);
        mm_free(frame->bc_list);
        mm_free(frame);
        return ret;
    }

    ret = test_vm_run_frame(core_id, frame);
    if (ret != EC_OK) {
        core_printf("[ctrl] test_vm run bc frame failed, ret[%d]\n", ret);
        mm_free(frame->bc_list);
        mm_free(frame);
        return ret;
    }

    return EC_OK;
}

int ctrl_testcore_add_core(u8 core_id)
{
    int ret;
    ret = test_vm_add(core_id);
    if (ret != EC_OK) {
        core_printf("[ctrl] test_vm add failed, ret[%d]\n", ret);
        goto error_exit;
    }

    ret = test_core_add(core_id);
    if (ret != EC_OK) {
        core_printf("[ctrl] test_core add failed, ret[%d]\n", ret);
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