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
#include "test_frame.h"
#include "mm.h"
#include "log.h"
#include "test_bc.h"

test_frame_s *test_frame_init(u32 bc_num)
{
    test_frame_s *frame = mm_malloc(sizeof(test_frame_s));
    int bc_list_size;

    if (frame == NULL) {
        core_printf("[test_frame] init alloc frame failed\n");
        return NULL;
    }
    mm_memset_s(frame, sizeof(*frame), 0, sizeof(*frame));

    bc_list_size = bc_num * sizeof(test_bc_s);
    frame->bc_list = mm_malloc(bc_list_size);
    if (frame->bc_list == NULL) {
        core_printf("[test_frame] init alloc bc list failed\n");
        mm_free(frame);
        return NULL;
    }
    mm_memset_s(frame->bc_list, bc_list_size, 0, bc_list_size);
    frame->bc_num = bc_num;
    frame->idx = 0;
    return frame;
}