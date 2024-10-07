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
#ifndef _REGISTER_H_
#define _REGISTER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "plat.h"

/**
 * @brief cfunc type define
 * 8bit:
 * [4-7]: type code
 *      type  |  type code
 *      ----  |  ---------
 *      void  |  0x0
 *      u8    |  0x1
 *      u16   |  0x2
 *      u32   |  0x3
 *      u64   |  0x4
 *      s8    |  0x5
 *      s16   |  0x6
 *      s32   |  0x7
 *      s64   |  0x8
 *      f32   |  0x9
 *      f64   |  0xa
 *
 * [0-3]: pointer layer
 */
enum cfunc_type_code_e {
    CFUNC_void = 0x0,
    CFUNC_u8   = 0x1,
    CFUNC_u16  = 0x2,
    CFUNC_u32  = 0x3,
    CFUNC_u64  = 0x4,
    CFUNC_s8   = 0x5,
    CFUNC_s16  = 0x6,
    CFUNC_s32  = 0x7,
    CFUNC_s64  = 0x8,
    CFUNC_f32  = 0x9,
    CFUNC_f64  = 0xa,
    /* alias */
    CFUNC_char   = CFUNC_s8,
    CFUNC_uchar  = CFUNC_u8,
    CFUNC_short  = CFUNC_s16,
    CFUNC_ushort = CFUNC_u16,
#if defined(IS_16_BIT_PLATFORM)
    CFUNC_int    = CFUNC_s16,
    CFUNC_uint   = CFUNC_u16,
    CFUNC_long   = CFUNC_s32,
    CFUNC_ulong  = CFUNC_u32,
#elif defined(IS_32_BIT_PLATFORM)
    CFUNC_int    = CFUNC_s32,
    CFUNC_uint   = CFUNC_u32,
    CFUNC_long   = CFUNC_s32,
    CFUNC_ulong  = CFUNC_u32,
#elif defined(IS_64_BIT_PLATFORM)
    CFUNC_int    = CFUNC_s32,
    CFUNC_uint   = CFUNC_u32,
    CFUNC_long   = CFUNC_s64,
    CFUNC_ulong  = CFUNC_u64,
#endif
    CFUNC_longlong  = CFUNC_s64,
    CFUNC_ulonglong = CFUNC_u64,
    CFUNC_float  = CFUNC_f32,
    CFUNC_double = CFUNC_f64,
};

enum cfunc_pointer_layer_e {
    CFUNC_P_LAYER_0 = 0x0,
    CFUNC_P_LAYER_1 = 0x1,
    CFUNC_P_LAYER_2 = 0x2,
    CFUNC_P_LAYER_3 = 0x3,
    CFUNC_P_LAYER_4 = 0x4,
    CFUNC_P_LAYER_5 = 0x5,
    CFUNC_P_LAYER_6 = 0x6,
    CFUNC_P_LAYER_7 = 0x7,
    CFUNC_P_LAYER_8 = 0x8,
    CFUNC_P_LAYER_9 = 0x9,
    CFUNC_P_LAYER_10 = 0xa,
    CFUNC_P_LAYER_11 = 0xb,
    CFUNC_P_LAYER_12 = 0xc,
    CFUNC_P_LAYER_13 = 0xd,
    CFUNC_P_LAYER_14 = 0xe,
    CFUNC_P_LAYER_15 = 0xf,
};

#define CFUNC_TYPE_CODE(type_name) CFUNC_##type_name
#define CFUNC_P_LAYER(p_layer) CFUNC_P_LAYER_##p_layer

#define ARG_N(                                         \
    _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10,  \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
    _61, _62, _63,   N, ...)                           \
    N

#define RSEQ_N                               \
    63, 62, 61, 60,                          \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50,  \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40,  \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30,  \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20,  \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  \
    9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define EXPAND(x) x
#define NARG_(...) EXPAND( ARG_N(__VA_ARGS__) )
#define NARG(...) NARG_(__VA_ARGS__, RSEQ_N)

typedef unsigned char leafpy_cfunc_type;

int leafpy_register_cfunc(const char *module, const char *pyfunc, const void *cfunc,
                          leafpy_cfunc_type ret_type, unsigned int arg_num, ...);

#ifdef __cplusplus
}
#endif
#endif