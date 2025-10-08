/*
 * MIT License
 *
 * Copyright (c) 2025 AZhao yuzhaowei2002@outlook.com
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
#ifndef _TEST_OBJ_TYPE_H_
#define _TEST_OBJ_TYPE_H_
#ifdef __cplusplus
extern "C" {
#endif

#define GLOBAL_OBJ_ID       0
#define GLOBAL_OBJ_NAME     "global_obj"
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

#ifdef __cplusplus
}
#endif
#endif