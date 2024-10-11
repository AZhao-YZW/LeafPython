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
#include "mm.h"
#include "leafpy_cfg.h"
#include "log.h"
#include "error.h"

#if (LEAFPY_USE_MEM_MODE == LEAFPY_USE_STATIC_MEM)
u8 g_static_mem[LEAFPY_MAX_MEM_SIZE] = {0};

void *leafpy_malloc(u32 size)
{
    return NULL;
}

void leafpy_free(void *ptr)
{
}

#elif (LEAFPY_USE_MEM_MODE == LEAFPY_USE_DYNAMIC_MEM)

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64)
#include <stdlib.h>

void *leafpy_malloc(u32 size)
{
    return malloc(size);
}

void leafpy_free(void *ptr)
{
    free(ptr);
}
#endif

#endif

void *mm_malloc(u32 size)
{
    void *ptr = leafpy_malloc(size);
    if (ptr == NULL) {
        core_printf("leafpy_malloc failed, size: %u\n", size);
    }
    return ptr;
}

void mm_free(void *ptr)
{
    leafpy_free(ptr);
}