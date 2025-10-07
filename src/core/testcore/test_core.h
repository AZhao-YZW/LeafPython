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
#ifndef _TEST_CORE_H_
#define _TEST_CORE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "test_data.h"

enum test_core_op_e {
    TEST_CORE_OP_NEW,
    TEST_CORE_OP_DEL,
    TEST_CORE_OP_FIND,
    TEST_CORE_OP_SET,
    TEST_CORE_OP_GET,
    TEST_CORE_OP_CVT,
    TEST_CORE_OP_CALC,
    TEST_CORE_OP_LOGIC,
    TEST_CORE_OP_REG,
    TEST_CORE_OP_CALL,
    TEST_CORE_OP_NUM,
    TEST_CORE_OP_MAX = 0xFF
};

typedef struct {
    u8 obj_type;            /* enum obj_type_e */
    u8 obj_subtype;
    u8 obj_name_len;
    u32 parent_id;
    const char *obj_name;   /* obj_name is unique under the same parent_id */
} test_core_op_NEW;

typedef struct {
    u32 obj_id;
} test_core_res_NEW;

typedef struct {
    u32 obj_id;
} test_core_op_DEL;

typedef struct {
    u8 obj_type;
    u8 obj_subtype;
    u32 obj_id;
    const void *obj_val;
} test_core_op_SET;

typedef struct {
    u32 obj_id;
} test_core_op_GET;

typedef struct {
    void *obj_val;
} test_core_res_GET;

typedef struct {
    u8 cur_type;
    u8 cur_subtype;
    u8 tgt_type;
    u8 tgt_subtype;
    u32 cur_obj_id;
    u32 tgt_parent_id;
} test_core_op_CVT;

typedef struct {
    void *obj;
} test_core_res_CVT;

typedef struct {
    u32 parent_id;
    const char *obj_name;
} test_core_op_FIND;

typedef struct {
    u32 obj_id;
} test_core_res_FIND;

enum test_core_calc_op_e {
    CALC_OP_ADD,
    CALC_OP_SUB,
    CALC_OP_MUL,
    CALC_OP_DIV,
    CALC_OP_NUM
};

typedef struct {
    u8 op;
    u8 obj1_type;
    u8 obj2_type;
    u8 obj1_subtype;
    u8 obj2_subtype;
    u32 obj1_id;
    u32 obj2_id;
    u32 val_len;
} test_core_op_CALC;

typedef struct {
    void *obj_val;
} test_core_res_CALC;

enum test_core_logic_op_e {
    // one object operation
    LOGIC_OP_LOGIC_NOT,
    ONE_LOGIC_OP_MAX,
    // two object operation
    LOGIC_OP_EQ = ONE_LOGIC_OP_MAX,
    LOGIC_OP_NE,
    LOGIC_OP_GE,
    LOGIC_OP_GT,
    LOGIC_OP_LE,
    LOGIC_OP_LT,
    LOGIC_OP_LOGIC_AND,
    LOGIC_OP_LOGIC_OR,
    TWO_LOGIC_OP_MAX,
    LOGIC_OP_NUM = TWO_LOGIC_OP_MAX
};

typedef struct {
    u8 op;
    u8 obj1_type;
    u8 obj2_type;
    u8 obj1_subtype;
    u8 obj2_subtype;
    u32 obj1_id;
    u32 obj2_id;
    u32 val_len;
} test_core_op_LOGIC;

typedef struct {
    void *obj_val;
} test_core_res_LOGIC;

typedef struct {
    u8 op;          /* enum test_core_op_e */
    u8 rsv[7];
    union {
        test_core_op_NEW op_new;
        test_core_op_DEL op_del;
        test_core_op_SET op_set;
        test_core_op_GET op_get;
        test_core_op_CVT op_cvt;
        test_core_op_FIND op_find;
        test_core_op_CALC op_calc;
        test_core_op_LOGIC op_logic;
    } info;
    union {
        u64 val;    /* support 8 bytes result data */
        test_core_res_NEW res_new;
        test_core_res_GET res_get;
        test_core_res_CVT res_cvt;
        test_core_res_FIND res_find;
        test_core_res_CALC res_calc;
        test_core_res_LOGIC res_logic;
    } result;
} test_core_op_info_s;

#define MAX_TEST_CORE_OP_INFO_SIZE  40
#define BUILD_CHECK_CORE_OP_INFO_SIZE() \
    BUILD_BUG_ON(sizeof(test_core_op_info_s) > MAX_TEST_CORE_OP_INFO_SIZE)

static inline void test_core_op_new_fill(u8 obj_type, u8 obj_subtype, u8 parent_id, const char *obj_name,
    test_core_op_info_s *op_info)
{
    op_info->op = TEST_CORE_OP_NEW;
    op_info->info.op_new.obj_type = obj_type;
    op_info->info.op_new.obj_subtype = obj_subtype;
    op_info->info.op_new.obj_name_len = sizeof(obj_name);
    op_info->info.op_new.parent_id = parent_id;
    op_info->info.op_new.obj_name = obj_name;
    op_info->result.res_new.obj_id = 0;
}

static inline void test_core_op_del_fill(u32 obj_id, test_core_op_info_s *op_info)
{
    op_info->op = TEST_CORE_OP_DEL;
    op_info->info.op_del.obj_id = obj_id;
}

static inline void test_core_op_set_fill(u8 obj_type, u8 obj_subtype, u32 obj_id, const void *set_val,
    test_core_op_info_s *op_info)
{
    op_info->op = TEST_CORE_OP_SET;
    op_info->info.op_set.obj_type = obj_type;
    op_info->info.op_set.obj_subtype = obj_subtype;
    op_info->info.op_set.obj_id = obj_id;
    op_info->info.op_set.obj_val = set_val;
}

static inline void test_core_op_get_fill(u32 obj_id, void *get_val, test_core_op_info_s *op_info)
{
    op_info->op = TEST_CORE_OP_GET;
    op_info->info.op_get.obj_id = obj_id;
    op_info->result.res_get.obj_val = get_val;
}

static inline void test_core_op_cacl_fill(enum test_core_calc_op_e calc_op,
        u8 t1, u8 st1, u8 id1, u8 t2, u8 st2, u8 id2, void *res_val, u32 val_len,
        test_core_op_info_s *op_info)
{
    op_info->op = TEST_CORE_OP_CALC;
    op_info->info.op_calc.op = calc_op;
    op_info->info.op_calc.obj1_type = t1;
    op_info->info.op_calc.obj1_subtype = st1;
    op_info->info.op_calc.obj1_id = id1;
    op_info->info.op_calc.obj2_type = t2;
    op_info->info.op_calc.obj2_subtype = st2;
    op_info->info.op_calc.obj2_id = id2;
    op_info->info.op_calc.val_len = val_len;
    op_info->result.res_calc.obj_val = res_val;
}

static inline void test_core_op_logic_fill(enum test_core_logic_op_e logic_op,
    u8 t1, u8 st1, u8 id1, u8 t2, u8 st2, u8 id2, void *res_val, u32 val_len,
    test_core_op_info_s *op_info)
{
    op_info->op = TEST_CORE_OP_LOGIC;
    op_info->info.op_logic.op = logic_op;
    op_info->info.op_logic.obj1_type = t1;
    op_info->info.op_logic.obj1_subtype = st1;
    op_info->info.op_logic.obj1_id = id1;
    op_info->info.op_logic.obj2_type = t2;
    op_info->info.op_logic.obj2_subtype = st2;
    op_info->info.op_logic.obj2_id = id2;
    op_info->info.op_logic.val_len = val_len;
    op_info->result.res_logic.obj_val = res_val;
}

typedef struct {
    int mock;
} register_cfunc_s;

typedef struct {
    struct list_head node;
    u8 core_id;
    global_obj_s *global_obj;
    register_cfunc_s *reg_cfunc;
} test_core_s;

int test_core_run(u8 core_id, test_core_op_info_s *op_info);
int test_core_init(u8 core_id);
int test_core_register_cfunc(register_cfunc_s *reg_cfunc);
test_core_s *test_core_get_core(u8 core_id);
int test_core_free(u8 core_id);
int test_core_add(u8 core_id);
int test_core_obj_get_type(u8 core_id, u32 obj_id, u8 *obj_type, u8 *obj_subtype);

void test_core_print_obj_list(u8 core_id);

#ifdef __cplusplus
}
#endif
#endif