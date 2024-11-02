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
    TEST_CORE_OP_NEW = 0,
    TEST_CORE_OP_DEL,
    TEST_CORE_OP_FIND,
    TEST_CORE_OP_ADD,
    TEST_CORE_OP_SUB,
    TEST_CORE_OP_MUL,
    TEST_CORE_OP_DIV,
    TEST_CORE_OP_PRINT,
    TEST_CORE_OP_CALL,
    TEST_CORE_OP_MAX
};

typedef struct {
    u8 obj_type;    /* enum test_data_obj_type_e */
    char *obj_name;
    u8 obj_name_len;
    u32 parent_id;
} test_core_op_NEW;

typedef struct {
    u32 obj_id;
} test_core_op_DEL;

typedef struct {
    u8 op;          /* enum test_core_op_e */
    union {
        test_core_op_NEW op_new;
        test_core_op_DEL op_del;
    } info;
} test_core_op_info_s;

typedef struct {
    struct list_head node;
    u8 core_id;
    global_obj_s *global_obj;
} test_core_s;

int test_core_run(u8 core_id, test_core_op_info_s *op_info);
int test_core_init(u8 core_id);
test_core_s *test_core_get_core(u8 core_id);
int test_core_free(u8 core_id);
int test_core_add(u8 core_id);

#ifdef __cplusplus
}
#endif
#endif