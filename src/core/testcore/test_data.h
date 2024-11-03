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
#ifndef _TEST_DATA_H_
#define _TEST_DATA_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "error_code.h"
#include "list.h"

#define GLOBAL_OBJ_LAYER    0
#define GLOBAL_OBJ_ID       0
#define GLOBAL_OBJ_NAME     "global_obj"
#define ROOT_OBJ_LAYER      1
#define ROOT_OBJ_ID         1
#define ROOT_OBJ_NAME       "root_obj"

enum test_data_obj_type_e {
    OBJ_TYPE_GLOBAL,   // global (special)
    OBJ_TYPE_ROOT,     // root (special)
    OBJ_TYPE_OBJECT,   // object
    OBJ_TYPE_NUMBER,   // Number
    OBJ_TYPE_STRING,   // String
    OBJ_TYPE_BOOL,     // bool
    OBJ_TYPE_LIST,     // List
    OBJ_TYPE_TUPLE,    // Tuple
    OBJ_TYPE_SET,      // Set
    OBJ_TYPE_DICT,     // Dict
    OBJ_TYPE_MAX,
};

#define NO_OBJ_SUBTYPE  0

enum test_data_number_type_e {
    NUM_TYPE_INT,       // int
    NUM_TYPE_FLOAT,     // float
    NUM_TYPE_BOOL,      // bool
    NUM_TYPE_COMPLEX,   // complex
};

#define OBJ_UNION_TYPE(t, st) ((u16)(t) << 8 | (u16)(st))
#define OBJ_UNION_TYPE_SIMPLE(t, st) OBJ_UNION_TYPE(OBJ_TYPE_##t, st)
#define OBJ_UNION_2_TYPE(t1, st1, t2, st2) (((u32)(t1) << 24 | (u32)(st1) << 16 | (u32)(t2) << 8 | (u32)(st2)))
#define OBJ_UNION_2_TYPE_SIMPLE(t1, st1, t2, st2) \
    OBJ_UNION_2_TYPE(OBJ_TYPE_##t1, st1, OBJ_TYPE_##t2, st2)

typedef struct {
    u16 obj_union_type;
    void (*set_obj_val)(void *obj, void *val);
    void (*get_obj_val)(void *val, void *obj);
} test_data_type_op_s;

#define DEFINE_SET_OBJ_VAL_FUNC(t, st, val_offset, val_type) \
static inline void set_obj_val_##t##_##st(void *obj, void *val) \
{ \
    *(val_type *)((u8 *)obj + (val_offset)) = *(val_type *)val; \
}

#define DEFINE_GET_OBJ_VAL_FUNC(t, st, val_offset, val_type) \
static inline void get_obj_val_##t##_##st(void *val, void *obj) \
{ \
    *(val_type *)val = *(val_type *)((u8 *)obj + (val_offset)); \
}

#define TEST_DATA_TYPE_OP_ELEMENT(t, st) \
    { OBJ_UNION_TYPE(t, st), set_obj_val_##t##_##st, get_obj_val_##t##_##st }

/**
 * Global Object
 * |-- Root Object
 *     |-- Number Object
 *     |-- String Object
 *     ...
 */

/**
 * @brief Object Base Attribute
 * @note All object has this attribute, and it MUST be 
 *       the first field of object type struct
 */
typedef struct {
    struct list_head node;
    u8 obj_type;        /* enum test_data_obj_type_e */
    u8 obj_subtype;
    bool free_flag;     /* false: using, true: can be freed */
    u8 layer;           /* 0: global, 1: root, >=2: others */
    u32 obj_id;         /* 0: global, 1: root, >=2: others */
    u32 parent_id;
    u32 child_num;
    char *obj_name;
} obj_base_attr_s;

#define MAX_TEST_DATA_OBJ_ATTR_SIZE 40
#define BUILD_CHECK_OBJ_ATTR_SIZE() BUILD_BUG_ON(sizeof(obj_base_attr_s) > MAX_TEST_DATA_OBJ_ATTR_SIZE)

/* Global Object */
typedef struct {
    obj_base_attr_s obj_attr;
    u32 obj_id_cnt;     /* total obj_id count */
    bool is_root_mounted;
} global_obj_s;

#define CAN_ROOT_MOUNTED(obj_type, is_mounted) ((obj_type) == OBJ_TYPE_ROOT && !(is_mounted))

/* Root Object */
typedef struct {
    obj_base_attr_s obj_attr;
} root_obj_s;

/* Base Object */
typedef struct {
    obj_base_attr_s obj_attr;
} object_obj_s;

/* Number Object */
typedef struct {
    object_obj_s obj_obj;
} number_obj_s;

typedef struct {
    number_obj_s num_obj;
    s64 val;
} int_obj_s;

typedef struct {
    number_obj_s num_obj;
    f64 val;
} float_obj_s;

/* bool Object */
typedef struct {
    number_obj_s num_obj;
    u8 val;
} bool_obj_s;

typedef struct {
    number_obj_s num_obj;
} complex_obj_s;

/* String Object */
typedef struct {
    object_obj_s obj_obj;
    char *val;
} string_obj_s;

/* List Object */
typedef struct {
    object_obj_s obj_obj;
} list_obj_s;

/* Tuple Object */
typedef struct {
    object_obj_s obj_obj;
} tuple_obj_s;

/* Set Object */
typedef struct {
    object_obj_s obj_obj;
} set_obj_s;

/* Dict Object */
typedef struct {
    object_obj_s obj_obj;
} dict_obj_s;

typedef union {
    global_obj_s    global_obj;
    root_obj_s      root_obj;
    object_obj_s    object_obj;
    number_obj_s    number_obj;
    int_obj_s       int_obj;
    float_obj_s     float_obj;
    complex_obj_s   complex_obj;
    bool_obj_s      bool_obj;
    string_obj_s    string_obj;
    list_obj_s      list_obj;
    tuple_obj_s     tuple_obj;
    set_obj_s       set_obj;
    dict_obj_s      dict_obj;
} test_data_obj_u;

#define MAX_TEST_DATA_OBJ_SIZE 128
#define BUILD_CHECK_OBJ_SIZE() BUILD_BUG_ON(sizeof(test_data_obj_u) > MAX_TEST_DATA_OBJ_SIZE)

/*****************************************************************************
 *                          Interface of test_data
 *****************************************************************************/
enum test_data_type_op_e {
    TYPE_OP_SET_OBJ_VAL,
    TYPE_OP_GET_OBJ_VAL,
    TYPE_OP_MAX
};

typedef struct {
    u8 op;          /* enum test_data_type_op_e */
    u8 obj_id;
    u8 obj_type;
    u8 obj_subtype;
    global_obj_s *global_obj;
    union {
        struct {
            void *val;
        } set_get_obj_val;
    } param;
} test_data_obj_op_info_s;

int test_data_init(global_obj_s **global_obj);
int test_data_free(global_obj_s **global_obj);
int test_data_obj_new(u8 obj_type, u8 obj_subtype, const char *obj_name, u32 parent_id,
                      global_obj_s *global_obj);
int test_data_obj_del(u32 obj_id, global_obj_s *global_obj);
int test_data_obj_get_id_by_name(const char *obj_name, u32 parent_id, global_obj_s *global_obj,
                                 u32 *obj_id);
int test_data_obj_op_proc(test_data_obj_op_info_s *info);
int test_data_obj_add(u32 obj1_id, u32 obj2_id, u32 val_len, global_obj_s *global_obj, void *obj_val);

void test_data_print_obj_list(global_obj_s *global_obj);

#ifdef __cplusplus
}
#endif
#endif