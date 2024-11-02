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
#ifndef _TYPE_H_
#define _TYPE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "plat.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
#if defined(IS_16_BIT_PLATFORM)
typedef unsigned long u32;
typedef signed long s32;
#elif defined(IS_32_BIT_PLATFORM) || defined(IS_64_BIT_PLATFORM)
typedef unsigned int u32;
typedef signed int s32;
#endif
typedef unsigned long long u64;
typedef signed long long s64;
typedef float f32;
typedef double f64;
#ifndef __cplusplus
typedef u8 bool;
#endif
#define true (u8)1
#define false (u8)0
#ifndef NULL
#define NULL ((void *)0)
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))
#define BIT(x) (1 << (x))

#ifdef __cplusplus
}
#endif
#endif