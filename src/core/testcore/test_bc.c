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
#include "test_bc.h"
#include "error_code.h"
#include "log.h"

void test_bc_proc_NOP(test_bc_s *bc)
{
}

void test_bc_proc_MOV(test_bc_s *bc)
{
    bc->arg1 = bc->arg2;
}

void test_bc_proc_INC(test_bc_s *bc)
{
    bc->arg1++;
}

void test_bc_proc_DEC(test_bc_s *bc)
{
    bc->arg1--;
}

void test_bc_proc_ADD(test_bc_s *bc)
{
    bc->arg1 = bc->arg2 + bc->arg3;
}

void test_bc_proc_SUB(test_bc_s *bc)
{
    bc->arg1 = bc->arg2 - bc->arg3;
}

void test_bc_proc_MUL(test_bc_s *bc)
{
    bc->arg1 = bc->arg2 * bc->arg3;
}

void test_bc_proc_DIV(test_bc_s *bc)
{
    bc->arg1 = bc->arg2 / bc->arg3;
}

void test_bc_proc_CMP(test_bc_s *bc)
{
    if (bc->arg2 == bc->arg3) {
        bc->arg1 = 0;
    } else if (bc->arg2 > bc->arg3) {
        bc->arg1 = 1;
    } else {
        bc->arg1 = -1;
    }
}

void test_bc_proc_JZ(test_bc_s *bc)
{
    if (bc->arg1 == 0) {
        bc->next_pos = bc->arg2;
    }
}

void test_bc_proc_JNZ(test_bc_s *bc)
{
    if (bc->arg1 != 0) {
        bc->next_pos = bc->arg2;
    }
}

void test_bc_proc_JMP(test_bc_s *bc)
{
    bc->next_pos = bc->arg1;
}

void test_bc_proc_PRINT(test_bc_s *bc)
{
    core_printf("PRINT: arg1: %u, arg2: %u, arg3: %u\n", bc->arg1, bc->arg2, bc->arg3);
}

void test_bc_proc_EXIT(test_bc_s *bc)
{
}

void test_bc_proc_CALL(test_bc_s *bc)
{   
}

void test_bc_proc_RET(test_bc_s *bc)
{   
}

void test_bc_proc_FUNC(test_bc_s *bc)
{   
}

struct {
    enum test_bc_op_e op;
    void (*proc)(test_bc_s *bc);
} g_test_bc_map[] = {
    { TEST_BC_NOP, test_bc_proc_NOP },
    { TEST_BC_MOV, test_bc_proc_MOV },
    { TEST_BC_INC, test_bc_proc_INC },
    { TEST_BC_DEC, test_bc_proc_DEC },
    { TEST_BC_ADD, test_bc_proc_ADD },
    { TEST_BC_SUB, test_bc_proc_SUB },
    { TEST_BC_MUL, test_bc_proc_MUL },
    { TEST_BC_DIV, test_bc_proc_DIV },
    { TEST_BC_CMP, test_bc_proc_CMP },
    { TEST_BC_JZ, test_bc_proc_JZ },
    { TEST_BC_JNZ, test_bc_proc_JNZ },
    { TEST_BC_JMP, test_bc_proc_JMP },
    { TEST_BC_PRINT, test_bc_proc_PRINT },
    { TEST_BC_EXIT, test_bc_proc_EXIT },
    { TEST_BC_CALL, test_bc_proc_CALL },
    { TEST_BC_RET, test_bc_proc_RET },
    { TEST_BC_FUNC, test_bc_proc_FUNC },
};

int test_bc_proc(test_bc_s *bc)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(g_test_bc_map); i++) {
        if (g_test_bc_map[i].op == bc->op) {
            g_test_bc_map[i].proc(bc);
            return EC_OK;
        }
    }
    log_printf(LOG_ERROR, "unknown op: %u\n", bc->op);
    return EC_UNSUPPORT_OP;
}