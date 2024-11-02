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
#include "list.h"

#define GLOBAL_OBJ_LAYER    0
#define GLOBAL_OBJ_ID       0
#define ROOT_OBJ_LAYER      1
#define ROOT_OBJ_ID         1

enum test_data_obj_type_e {
    OBJ_TYPE_GLOBAL,   // global (special)
    OBJ_TYPE_OBJECT,   // object
    OBJ_TYPE_NUMBER,   // Number
    OBJ_TYPE_STRING,   // String
    OBJ_TYPE_BOOL,     // bool
    OBJ_TYPE_LIST,     // List
    OBJ_TYPE_TUPLE,    // Tuple
    OBJ_TYPE_SET,      // Set
    OBJ_TYPE_DICT,     // Dict
};

enum test_number_type_e {
    NUM_TYPE_INT,       // int
    NUM_TYPE_FLOAT,     // float
    NUM_TYPE_BOOL,      // bool
    NUM_TYPE_COMPLEX,   // complex
};

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
    bool free_flag;     /* false: using, true: can be freed */
    u8 layer;           /* 0: global, 1: root, >=2: others */
    u32 obj_id;         /* 0: global, 1: root, >=2: others */
    u32 parent_id;
    u32 child_num;
    char *obj_name;
} obj_base_attr_s;

/* Global Object */
typedef struct {
    obj_base_attr_s obj_attr;
    u32 obj_id_cnt;     /* total obj_id count */
} global_obj_s;

/* Root Object */
typedef struct {
    obj_base_attr_s obj_attr;
} object_obj_s;

/* Number Object */
typedef struct {
    obj_base_attr_s obj_attr;
} number_obj_s;

typedef struct {
    number_obj_s num_obj;
} int_obj_s;

typedef struct {
    number_obj_s num_obj;
} float_obj_s;

typedef struct {
    number_obj_s num_obj;
} complex_obj_s;

/* bool Object */
typedef struct {
    number_obj_s num_obj;
} bool_obj_s;

/* String Object */
typedef struct {
    obj_base_attr_s obj_attr;
} string_obj_s;

/* List Object */
typedef struct {
    obj_base_attr_s obj_attr;
} list_obj_s;

/* Tuple Object */
typedef struct {
    obj_base_attr_s obj_attr;
} tuple_obj_s;

/* Set Object */
typedef struct {
    obj_base_attr_s obj_attr;
} set_obj_s;

/* Dict Object */
typedef struct {
    obj_base_attr_s obj_attr;
} dict_obj_s;

int test_data_init(global_obj_s **global_obj);
int test_data_obj_new(u8 obj_type, char *obj_name, u32 parent_id, global_obj_s *global_obj);
int test_data_obj_del(u32 obj_id, global_obj_s *global_obj);
int test_data_obj_get(const char *obj_name, global_obj_s *global_obj, u32 *obj_id);
void test_data_print_obj_list(global_obj_s *global_obj);

#ifdef __cplusplus
}
#endif
#endif