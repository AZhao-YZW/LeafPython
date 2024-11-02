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
#ifndef _LEAF_CFG_H_
#define _LEAF_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief leafpython default core type
 * 0: leafpy
 * 1: testcore
 */
#define CORE_TYPE_LEAFPY            0
#define CORE_TYPE_TESTCORE          1
#define LEAFPY_DEFAULT_CORE_TYPE    CORE_TYPE_TESTCORE

/**
 * @brief enable core type switch
 * 0: disable
 * 1: enable
 */
#define ENABLE_CORE_LEAFPY          1
#define ENABLE_CORE_TESTCORE        1

/**
 * @brief leafpython default core id
 */
#define LEAFPY_DEFAULT_CORE_ID      0

/**
 * @brief leafpython log level
 * 0: LOG_DEBUG
 * 1: LOG_INFO
 * 2: LOG_WARN
 * 3: LOG_ERROR
 * 4: LOG_FATAL
 */
#define LEAFPY_LOG_LEVEL            0

/**
 * @brief leafpython core log enable
 * 0: disable
 * 1: enable
 */
#define LEAFPY_CORE_LOG_ENABLE      1

#define LEAFPY_CORE_NAME_LEAFPY     "leafpy"
#define LEAFPY_CORE_NAME_TESTCORE   "testcore"

/**
 * @brief leafpython use memory mode
 * 0: use static memory
 * 1: use dynamic memory
 */
#define LEAFPY_USE_STATIC_MEM       0
#define LEAFPY_USE_DYNAMIC_MEM      1
#define LEAFPY_USE_MEM_MODE         LEAFPY_USE_DYNAMIC_MEM

#if (LEAFPY_USE_MEM_MODE == LEAFPY_USE_STATIC_MEM)
#define LEAFPY_MAX_MEM_SIZE         (1024 * 4)
#elif (LEAFPY_USE_MEM_MODE == LEAFPY_USE_DYNAMIC_MEM)
#ifdef IS_16_BIT_PLATFORM
void *leafpy_malloc(unsigned long size) __attribute__((weak));
#elif defined(IS_32_BIT_PLATFORM) || defined(IS_64_BIT_PLATFORM)
void *leafpy_malloc(unsigned int size) __attribute__((weak));
#endif
void leafpy_free(void *ptr) __attribute__((weak));
#endif


#ifdef __cplusplus
}
#endif
#endif