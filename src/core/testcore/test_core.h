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
} test_core_op_DEL;

typedef struct {
    u8 obj_type;
    u8 obj_subtype;
    u32 obj_id;
    void *obj_val;
} test_core_op_SET;

typedef struct {
    u8 obj_type;
    u8 obj_subtype;
    u32 obj_id;
} test_core_op_GET;

typedef struct {
    void *obj_val;
} test_core_res_GET;

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
    bool ret_bool;;
} test_core_res_LOGIC;

typedef struct {
    u8 op;          /* enum test_core_op_e */
    u8 rsv[7];
    union {
        test_core_op_NEW op_new;
        test_core_op_DEL op_del;
        test_core_op_SET op_set;
        test_core_op_GET op_get;
        test_core_op_FIND op_find;
        test_core_op_CALC op_calc;
        test_core_op_LOGIC op_logic;
    } info;
    union {
        u64 val;    /* support 8 bytes result data */
        test_core_res_GET res_get;
        test_core_res_FIND res_find;
        test_core_res_CALC res_calc;
        test_core_res_LOGIC res_logic;
    } result;
} test_core_op_info_s;

#define MAX_TEST_CORE_OP_INFO_SIZE  40
#define BUILD_CHECK_CORE_OP_INFO_SIZE() \
    BUILD_BUG_ON(sizeof(test_core_op_info_s) > MAX_TEST_CORE_OP_INFO_SIZE)

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
void test_core_print_obj_list(u8 core_id);

#ifdef __cplusplus
}
#endif
#endif