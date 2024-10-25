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
#ifndef _TEST_BC_H_
#define _TEST_BC_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"

enum test_bc_op_e {
    TEST_BC_NOP = 0,
    TEST_BC_MOV,
    TEST_BC_INC,
    TEST_BC_DEC,
    TEST_BC_ADD,
    TEST_BC_SUB,
    TEST_BC_MUL,
    TEST_BC_DIV,
    TEST_BC_CMP,
    TEST_BC_JZ,
    TEST_BC_JNZ,
    TEST_BC_JMP,
    TEST_BC_PRINT,
    TEST_BC_EXIT,
    TEST_BC_CALL,
    TEST_BC_RET,
    TEST_BC_FUNC,
    TEST_BC_MAX
};

typedef struct test_bc_s {
    u8 op;      /* enum test_bc_op_e */
    u32 arg1;
    u32 arg2;
    u32 arg3;
    u32 pos;
    u32 next_pos;
} test_bc_s;

int test_bc_proc(test_bc_s *bc);

#ifdef __cplusplus
}
#endif
#endif