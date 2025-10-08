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
    TEST_BC_NEW,
    TEST_BC_DEL,
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

enum test_bc_MOV_op_e {
    TEST_BC_MOV_OBJ = 0,
    TEST_BC_MOV_VAL,
    TEST_BC_MOV_MAX
};

enum test_bc_arg_type_e {
    TEST_BC_ARG_TYPE_NEW = 0,
    TEST_BC_ARG_TYPE_1_ARG_1_OBJ,
    TEST_BC_ARG_TYPE_2_ARG_1_OBJ,
    TEST_BC_ARG_TYPE_2_ARG_2_OBJ,
    TEST_BC_ARG_TYPE_3_ARG_1_OBJ,
    TEST_BC_ARG_TYPE_3_ARG_2_OBJ,
    TEST_BC_ARG_TYPE_3_ARG_3_OBJ,
    TEST_BC_ARG_TYPE_MAX
};

typedef struct {
    const char *obj_name;
} test_bc_NEW;

typedef struct {
    u32 obj_id;
} test_bc_NEW_res;

typedef struct {
    u32 obj_id;
} test_bc_1_arg;

typedef struct {
    u32 obj_id1;
    union {
        u32 obj_id2;
        const void *val;
    };
} test_bc_2_args;

typedef struct {
    u32 obj_id1;
    union {
        struct {
            u32 obj_id2;
            u32 obj_id3;
        } t1;
        struct {
            u32 obj_id2;
            const void *val1;
        } t2;
        struct {
            const void *val1;
            const void *val2;
        } t3;
    };
} test_bc_3_args;

typedef struct {
    u8 op;          /* enum test_bc_op_e */
    u8 sub_op;
    u8 arg_type;    /* enum test_bc_arg_type_e*/
    u8 core_id;
    u32 parent_id;
    struct {
        struct {
            u8 obj_type1;
            u8 obj_subtype1;
            u8 obj_type2;
            u8 obj_subtype2;
            u8 obj_type3;
            u8 obj_subtype3;
            u8 rsv[2];
        };
        union {
            u64 arg[3];
            test_bc_NEW bc_new;
            test_bc_1_arg bc_1_arg;
            test_bc_2_args bc_2_args;
            test_bc_3_args bc_3_args;
        };
    } args;
    union {
        u64 bc_res;
        test_bc_NEW_res bc_new_res;
    };
    // used by upper layer, bc do not use it
    u64 pos;
    u64 next_pos;
    u32 obj_vm_id1;
    u32 obj_vm_id2;
    u32 obj_vm_id3;
    u8 rsv2[4];
    const void *exp_val;    // debug check val
} test_bc_s;

#define BUILD_CHECK_TEST_BC_ARGS_SIZE() \
    BUILD_BUG_ON(sizeof(((test_bc_s*)0)->args) != 4 * sizeof(u64))

static inline void test_bc_common_fill(enum test_bc_op_e bc_op, u8 core_id, u32 parent_id, test_bc_s *bc)
{
    bc->op = bc_op;
    bc->core_id = core_id;
    bc->parent_id = parent_id;
    bc->bc_res = 0;
}

static inline void test_bc_new_fill(u8 t, u8 st, const char *obj_name, u8 c, u8 p, test_bc_s *bc)
{
    test_bc_common_fill(TEST_BC_NEW, c, p, bc);
    bc->arg_type = TEST_BC_ARG_TYPE_NEW;
    bc->args.obj_type1 = t;
    bc->args.obj_subtype1 = st;
    bc->args.bc_new.obj_name = obj_name;
}

static inline void test_bc_del_fill(u8 c, u8 p, test_bc_s *bc)
{
    test_bc_common_fill(TEST_BC_DEL, c, p, bc);
    bc->arg_type = TEST_BC_ARG_TYPE_1_ARG_1_OBJ;
}

static inline void test_bc_mov_obj_fill(u8 c, u8 p, test_bc_s *bc)
{
    test_bc_common_fill(TEST_BC_MOV, c, p, bc);
    bc->arg_type = TEST_BC_ARG_TYPE_2_ARG_2_OBJ;
    bc->sub_op = TEST_BC_MOV_OBJ;
}

static inline void test_bc_mov_val_fill(const void *val, u8 c, u8 p, test_bc_s *bc)
{
    test_bc_common_fill(TEST_BC_MOV, c, p, bc);
    bc->arg_type = TEST_BC_ARG_TYPE_1_ARG_1_OBJ;
    bc->sub_op = TEST_BC_MOV_VAL;
    bc->args.bc_2_args.val = val;
}

static inline void test_bc_inc_fill(u8 c, u8 p, test_bc_s *bc)
{
    test_bc_common_fill(TEST_BC_INC, c, p, bc);
    bc->arg_type = TEST_BC_ARG_TYPE_1_ARG_1_OBJ;
}

static inline void test_bc_dec_fill(u8 c, u8 p, test_bc_s *bc)
{
    test_bc_common_fill(TEST_BC_DEC, c, p, bc);
    bc->arg_type = TEST_BC_ARG_TYPE_1_ARG_1_OBJ;
}

static inline void test_bc_calc_fill(enum test_bc_op_e bc_op, u8 c, u8 p, test_bc_s *bc)
{
    test_bc_common_fill(bc_op, c, p, bc);
    bc->arg_type = TEST_BC_ARG_TYPE_3_ARG_3_OBJ;
}

typedef int (*test_bc_cb_t)(test_bc_s *bc);

int test_bc_proc(test_bc_s *bc, test_bc_cb_t pre, test_bc_cb_t post);
void test_bc_debug_print(test_bc_s *bc);

#ifdef __cplusplus
}
#endif
#endif