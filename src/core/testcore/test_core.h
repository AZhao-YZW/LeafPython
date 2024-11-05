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
    TEST_CORE_OP_NEW        = 0,
    TEST_CORE_OP_DEL        = 1,
    TEST_CORE_OP_FIND       = 2,
    TEST_CORE_OP_SET        = 3,
    TEST_CORE_OP_GET        = 4,
    TEST_CORE_OP_CALC       = 5,
    TEST_CORE_OP_PRINT      = 6,
    TEST_CORE_OP_REG        = 7,
    TEST_CORE_OP_CALL       = 8,
    TEST_CORE_OP_MAX        = 0xFF
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
    CALC_OP_MAX
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
    } info;
    union {
        u64 val;    /* support 8 bytes result data */
        test_core_res_GET res_get;
        test_core_res_FIND res_find;
        test_core_res_CALC res_calc;
    } result;
} test_core_op_info_s;

#define MAX_TEST_CORE_OP_INFO_SIZE  40
#define BUILD_CHECK_CORE_OP_INFO_SIZE() \
    BUILD_BUG_ON(sizeof(test_core_op_info_s) > MAX_TEST_CORE_OP_INFO_SIZE)

typedef struct {
    int (*eq)(u8 obj1_id, u8 obj2_id);
    int (*ne)(u8 obj1_id, u8 obj2_id);
    int (*ge)(u8 obj1_id, u8 obj2_id);
    int (*gt)(u8 obj1_id, u8 obj2_id);
    int (*le)(u8 obj1_id, u8 obj2_id);
    int (*lt)(u8 obj1_id, u8 obj2_id);
    int (*logic_and)(u8 obj1_id, u8 obj2_id);
    int (*logic_or)(u8 obj1_id, u8 obj2_id);
    int (*logic_not)(u8 obj_id);
    int (*add)(u8 obj1_id, u8 obj2_id);
    int (*sub)(u8 obj1_id, u8 obj2_id);
    int (*mul)(u8 obj1_id, u8 obj2_id);
    int (*div)(u8 obj1_id, u8 obj2_id);
} builtin_func_s;

typedef struct {
    int mock;
} register_cfunc_s;

typedef struct {
    struct list_head node;
    u8 core_id;
    global_obj_s *global_obj;
    builtin_func_s *builtin_func;
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