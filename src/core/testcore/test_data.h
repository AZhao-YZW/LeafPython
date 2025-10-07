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

enum obj_type_e {
    OBJ_TYPE_GLOBAL,   // global (special)
    OBJ_TYPE_ROOT,     // root (special)
    OBJ_TYPE_OBJECT,   // object
    OBJ_TYPE_NUMBER,   // Number
    OBJ_TYPE_STRING,   // String
    OBJ_TYPE_LIST,     // List
    OBJ_TYPE_TUPLE,    // Tuple
    OBJ_TYPE_SET,      // Set
    OBJ_TYPE_DICT,     // Dict
    OBJ_TYPE_MAX,
};

#define NO_OBJ_SUBTYPE  0

enum number_type_e {
    NUM_TYPE_INT,       // int
    NUM_TYPE_FLOAT,     // float
    NUM_TYPE_BOOL,      // bool
    NUM_TYPE_COMPLEX,   // complex
};

#define OBJ_UNION_TYPE(t, st) ((u16)(t) << 8 | (u16)(st))
#define OBJ_UNION_TYPE_SIMPLE(t, st) OBJ_UNION_TYPE(OBJ_TYPE_##t, st)
#define OBJ_UNION_2_TYPE(t1, st1, t2, st2) \
    (((u32)(t1) << 24 | (u32)(st1) << 16 | (u32)(t2) << 8 | (u32)(st2)))
#define OBJ_UNION_2_TYPE_SIMPLE(t1, st1, t2, st2) \
    OBJ_UNION_2_TYPE(OBJ_TYPE_##t1, st1, OBJ_TYPE_##t2, st2)

/*****************************************************************************
 *                         object operation code
 *****************************************************************************/
enum test_data_obj_op_e {
    // one object operation
    OBJ_OP_SET,
    OBJ_OP_GET,
    OBJ_OP_LOGIC_NOT,
    ONE_OBJ_OP_MAX,
    // two object operation
    OBJ_OP_ADD = ONE_OBJ_OP_MAX,
    OBJ_OP_SUB,
    OBJ_OP_MUL,
    OBJ_OP_DIV,
    OBJ_OP_EQ,
    OBJ_OP_NE,
    OBJ_OP_GE,
    OBJ_OP_GT,
    OBJ_OP_LE,
    OBJ_OP_LT,
    OBJ_OP_LOGIC_AND,
    OBJ_OP_LOGIC_OR,
    TWO_OBJ_OP_MAX
};

#define OBJ_VAL(obj, loc, vt) (*(vt *)((u8 *)(obj) + (loc)))
#define OBJ_FUNC_NAME(op, t, st) op##_##t##_##st

/*****************************************************************************
 *                          one object operation
 *****************************************************************************/
typedef struct {
    u16 obj_ut;     /* obj union type */
    union {
        int (*op[ONE_OBJ_OP_MAX])(void *obj, void *val);
        struct {
            int (*set)(void *obj, void *val);
            int (*get)(void *obj, void *val);
            int (*logic_not)(void *obj, void *val);
        };
    };
} one_obj_type_op_s;

#define DEFINE_OBJ_SET_FUNC(t, st, loc, vt) \
static int set_##t##_##st(void *obj, void *val) { \
    OBJ_VAL(obj, loc, vt) = *(vt *)val; \
    return EC_OK; \
}
#define DEFINE_OBJ_GET_FUNC(t, st, loc, vt) \
static int get_##t##_##st(void *obj, void *val) { \
    *(vt *)val = OBJ_VAL(obj, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_LOGIC_NOT_FUNC(t, st, loc, vt) \
static int logic_not_##t##_##st(void *obj, void *val) { \
    *(bool *)val = !OBJ_VAL(obj, loc, vt) ? true : false; \
    return EC_OK; \
}

#define ONE_OBJ_OP_ELE(t, st) \
    { OBJ_UNION_TYPE(t, st), set_##t##_##st, get_##t##_##st, logic_not_##t##_##st }

#define DEFINE_ONE_OBJ_NA_FUNC(op, t, st) \
static int op##_##t##_##st(void *obj, void *val) { \
    return EC_OBJ_TYPE_INVALID; \
}

/*****************************************************************************
 *                          two object operation
 *****************************************************************************/
typedef struct {
    u16 obj1_ut;    /*  obj1 union type */
    u16 obj2_ut;    /*  obj2 union type */
    union {
        int (*op[TWO_OBJ_OP_MAX - ONE_OBJ_OP_MAX])(void *obj1, void *obj2, void *val, u32 val_len);
        struct {
            int (*add)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*sub)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*mul)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*div)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*eq)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*ne)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*ge)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*gt)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*le)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*lt)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*logic_and)(void *obj1, void *obj2, void *val, u32 val_len);
            int (*logic_or)(void *obj1, void *obj2, void *val, u32 val_len);
        };
    };
} two_obj_type_op_s;

#define DEFINE_OBJ_ADD_FUNC(t, st, loc, vt) \
static int add_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(vt *)val = OBJ_VAL(obj1, loc, vt) + OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_SUB_FUNC(t, st, loc, vt) \
static int sub_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(vt *)val = OBJ_VAL(obj1, loc, vt) - OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_MUL_FUNC(t, st, loc, vt) \
static int mul_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(vt *)val = OBJ_VAL(obj1, loc, vt) * OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_DIV_FUNC(t, st, loc, vt) \
static int div_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(vt *)val = OBJ_VAL(obj1, loc, vt) / OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_EQ_FUNC(t, st, loc, vt) \
static int eq_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(bool *)val = OBJ_VAL(obj1, loc, vt) == OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_NE_FUNC(t, st, loc, vt) \
static int ne_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(bool *)val = OBJ_VAL(obj1, loc, vt) != OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_GE_FUNC(t, st, loc, vt) \
static int ge_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(bool *)val = OBJ_VAL(obj1, loc, vt) >= OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_GT_FUNC(t, st, loc, vt) \
static int gt_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(bool *)val = OBJ_VAL(obj1, loc, vt) > OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_LE_FUNC(t, st, loc, vt) \
static int le_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(bool *)val = OBJ_VAL(obj1, loc, vt) <= OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_LT_FUNC(t, st, loc, vt) \
static int lt_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(bool *)val = OBJ_VAL(obj1, loc, vt) < OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_LOGIC_AND_FUNC(t, st, loc, vt) \
static int logic_and_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(vt *)val = !OBJ_VAL(obj1, loc, vt) ? OBJ_VAL(obj1, loc, vt) : OBJ_VAL(obj2, loc, vt); \
    return EC_OK; \
}
#define DEFINE_OBJ_LOGIC_OR_FUNC(t, st, loc, vt) \
static int logic_or_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    *(vt *)val = !OBJ_VAL(obj1, loc, vt) ? OBJ_VAL(obj2, loc, vt) : OBJ_VAL(obj1, loc, vt); \
    return EC_OK; \
}

#define TWO_OBJ_OP_ELE(t, st) \
    { OBJ_UNION_TYPE(t, st), OBJ_UNION_TYPE(t, st), add_##t##_##st, \
      sub_##t##_##st, mul_##t##_##st, div_##t##_##st, eq_##t##_##st, \
      ne_##t##_##st, ge_##t##_##st, gt_##t##_##st, le_##t##_##st, \
      lt_##t##_##st, logic_and_##t##_##st, logic_or_##t##_##st }

#define DEFINE_TWO_OBJ_NA_FUNC(op, t, st) \
static int op##_##t##_##st(void *obj1, void *obj2, void *val, u32 val_len) { \
    return EC_OBJ_TYPE_INVALID; \
}

/*****************************************************************************
 *                      all object operation functions
 *****************************************************************************/
/* obj_type,  obj_subtype,  val_offset,  val_type */
#define DEFINE_OBJ_ALL_OP_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_SET_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_GET_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_LOGIC_NOT_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_ADD_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_SUB_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_MUL_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_DIV_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_EQ_FUNC(t, st, loc, vt)  \
    DEFINE_OBJ_NE_FUNC(t, st, loc, vt)  \
    DEFINE_OBJ_GE_FUNC(t, st, loc, vt)  \
    DEFINE_OBJ_GT_FUNC(t, st, loc, vt)  \
    DEFINE_OBJ_LE_FUNC(t, st, loc, vt)  \
    DEFINE_OBJ_LT_FUNC(t, st, loc, vt)  \
    DEFINE_OBJ_LOGIC_AND_FUNC(t, st, loc, vt) \
    DEFINE_OBJ_LOGIC_OR_FUNC(t, st, loc, vt)

/*****************************************************************************
 *                           object structures
 *****************************************************************************/
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
    u8 obj_type;        /* enum obj_type_e */
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

/**
 * @brief Object C Funcion Attribute
 * @note All object has this attribute, and this attribute 
 *       only represents C functions registered to the core,
 *       Python functions deal in vm. it MUST be the second
 *       field of object type struct
 */
typedef struct {
    struct list_head node;
    u32 cfunc_id;
    void *cfunc;
} obj_cfunc_attr_s;

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
    obj_cfunc_attr_s cfunc_attr;
} root_obj_s;

/* Base Object */
typedef struct {
    obj_base_attr_s obj_attr;
    obj_cfunc_attr_s cfunc_attr;
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
    u32 val_len;
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

typedef struct {
    u8 op;          /* enum test_data_obj_op_e */
    union {
        struct {
            u8 obj_type;
            u8 obj_subtype;
            u32 obj_id;
        } one_obj;
        struct {
            u8 obj1_type;
            u8 obj2_type;
            u8 obj1_subtype;
            u8 obj2_subtype;
            u32 obj1_id;
            u32 obj2_id;
        } two_obj;
    };
    u32 ret_val_len;
    void *ret_val;  /* return value */
    global_obj_s *global_obj;
 } obj_op_info_s;

#define MAX_OBJ_OP_INFO_SIZE  40
#define BUILD_CHECK_OBJ_OP_INFO_SIZE() \
    BUILD_BUG_ON(sizeof(obj_op_info_s) > MAX_OBJ_OP_INFO_SIZE)

int test_data_init(global_obj_s **global_obj);
int test_data_free(global_obj_s **global_obj);
int test_data_obj_op_proc(obj_op_info_s *info);
int test_data_obj_new(u8 obj_type, u8 obj_subtype, const char *obj_name, u32 parent_id,
                      global_obj_s *global_obj, u32 *ret_obj_id);
int test_data_obj_del(u32 obj_id, global_obj_s *global_obj);
int test_data_obj_get_id_by_name(const char *obj_name, u32 parent_id, global_obj_s *global_obj,
                                 u32 *obj_id);
int test_data_obj_get_name_by_id(u32 obj_id, u8 obj_name_len, global_obj_s *global_obj, char **obj_name);
int test_data_obj_get_name_len_by_id(u32 obj_id, global_obj_s *global_obj, u8 *obj_name_len);
int test_data_obj_get_type_by_id(u32 obj_id, global_obj_s *global_obj, u8 *obj_type, u8 *obj_subtype);
void test_data_print_obj_list(global_obj_s *global_obj);

#ifdef __cplusplus
}
#endif
#endif