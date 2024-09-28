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

// 定义内存堆
static u8 *heap_base;
static u32 heap_size;
static u8 *free_ptr;

// 初始化内存堆
void mm_init(u8 *heap, u32 size)
{
    heap_base = heap;
    heap_size = size;
    free_ptr = heap_base;
}

void mm_memset(void *dst, u8 val, u32 size)
{
    u8 *ptr = (u8 *)dst;
    
    while (size--) {
        *ptr = val;
        ptr++;
    }
}

void mm_memcpy(void *dst, void *src, u32 size)
{
    u8 *dst_ptr = (u8 *)dst;
    const u8 *src_ptr = (const u8 *)src;

    while (size--) {
        *dst_ptr = *src_ptr;
        dst_ptr++;
        src_ptr++;
    }
}

void mm_memcpy_s(void *dst, u32 dst_size, void *src, u32 size)
{
    u8 *dst_ptr = (u8 *)dst;
    const u8 *src_ptr = (const u8 *)src;

    while (size--) {
        if (dst_ptr - (u8 *)dst >= dst_size) {
            return; // 目标缓冲区溢出
        }
        *dst_ptr = *src_ptr;
        dst_ptr++;
        src_ptr++;
    }
}

// 分配内存
void *mm_malloc(u32 size)
{
    if (free_ptr + size > heap_base + heap_size) {
        return NULL; // 内存不足
    }

    void *result = free_ptr;
    free_ptr += size;
    return result;
}

// 分配并初始化为零的内存
void *mm_calloc(u32 num, u32 size)
{
    void *ptr = mm_malloc(num * size);
    if (ptr != NULL) {
        memset(ptr, 0, num * size);
    }
    return ptr;
}

// 重新分配内存
void *mm_realloc(void *ptr, u32 new_size)
{
    if (ptr == NULL) {
        return mm_malloc(new_size);
    }

    u32 old_size = (u8 *)ptr - heap_base;
    if (old_size >= new_size) {
        return ptr; // 不需要重新分配
    }

    void *new_ptr = mm_malloc(new_size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, ptr, old_size);
        mm_free(ptr);
    }
    return new_ptr;
}

// 释放内存
void mm_free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }

    u32 offset = (u8 *)ptr - heap_base;
    if (offset < heap_size) {
        free_ptr = (u8 *)ptr;
    }
}