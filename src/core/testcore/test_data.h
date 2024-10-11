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
#ifndef _TEST_FRAME_H_
#define _TEST_FRAME_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "list.h"

#define GLOBAL_OBJ_ID   0

enum test_data_type_e {
    DATA_TYPE_OBJECT,   // object
    DATA_TYPE_NUMBER,   // Number
    DATA_TYPE_STRING,   // String
    DATA_TYPE_BOOL,     // bool
    DATA_TYPE_LIST,     // List
    DATA_TYPE_TUPLE,    // Tuple
    DATA_TYPE_SET,      // Set
    DATA_TYPE_DICT,     // Dict
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

/* Base Object */
typedef struct {
    u8 obj_type : 4;
    u8 free_flag : 1;   /* 0: using, 1: can be freed */
    u8 rsv1 : 3;
    u8 rsv2[3];
    u32 obj_id;
    u32 parent_id;
    u32 child_num;
    char *obj_name;
} base_obj_s;

/* Global Object */
typedef struct {
    base_obj_s base_obj;
} global_obj_s;

/* Root Object */
typedef struct {
    base_obj_s base_obj;
} root_obj_s;

/* Number Object */
typedef struct {
    base_obj_s base_obj;
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
    base_obj_s base_obj;
} string_obj_s;

/* List Object */
typedef struct {
    base_obj_s base_obj;
} list_obj_s;

/* Tuple Object */
typedef struct {
    base_obj_s base_obj;
} tuple_obj_s;

/* Set Object */
typedef struct {
    base_obj_s base_obj;
} set_obj_s;

/* Dict Object */
typedef struct {
    base_obj_s base_obj;
} dict_obj_s;

int test_data_init(void);

#ifdef __cplusplus
}
#endif
#endif