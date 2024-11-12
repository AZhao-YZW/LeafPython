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
#include "test_bc.h"

enum frame_queue_e {
    TEST_FRAME_QUEUE_0
};

typedef struct {
    struct list_head node;
    u32 bc_num;
    u32 idx;
    test_bc_s *bc_list;
} test_frame_s;

typedef struct {
    int (*run_frame)(u8 core_id, test_frame_s *frame);
} test_frame_callback_s;

typedef struct {
    struct list_head mng_node;
    struct list_head frame_head;
    u8 frame_queue_id;
    u8 core_id;
    test_frame_callback_s cb;
} test_frame_mng_s;

/**
 * the workflow is:
 *   1. create a frame by test_frame_create()
 *   2. enqueue the frame by test_frame_enqueue()
 *   3. dequeue the frame after enqueue (and now it's immediately)
 *   4. run the frame by run_frame() which was registered from vm
 *   5. free the frame by test_frame_free() after run_frame()
 */
int test_frame_register(u8 frame_queue_id, u8 core_id, test_frame_callback_s *cb);
test_frame_s *test_frame_create(u32 bc_num);
int test_frame_enqueue(u8 frame_queue_id, test_frame_s *frame);

/* free all frames and queues */
void test_frame_free(void);

#ifdef __cplusplus
}
#endif
#endif