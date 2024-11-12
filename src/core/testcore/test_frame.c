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
#include "error_code.h"
#include "log.h"
#include "test_bc.h"

test_frame_s *test_frame_create(u32 bc_num)
{
    test_frame_s *frame = NULL;
    int bc_list_size;

    if (bc_num == 0) {
        core_log("[test_frame] bc_num is 0\n");
        return NULL;
    }

    frame = mm_malloc(sizeof(test_frame_s));
    if (frame == NULL || bc_num == 0) {
        core_log("[test_frame] alloc frame failed\n");
        return NULL;
    }
    mm_memset_s(frame, sizeof(*frame), 0, sizeof(*frame));

    bc_list_size = bc_num * sizeof(test_bc_s);
    frame->bc_list = mm_malloc(bc_list_size);
    if (frame->bc_list == NULL) {
        core_log("[test_frame] alloc bc list failed\n");
        mm_free(frame);
        return NULL;
    }
    mm_memset_s(frame->bc_list, bc_list_size, 0, bc_list_size);
    frame->bc_num = bc_num;
    frame->idx = 0;
    return frame;
}

static LIST_HEAD(g_frame_mng_list);

static int test_frame_dequeue(test_frame_mng_s *frame_mng)
{
    test_frame_s *frame = NULL;
    test_frame_s *next = NULL;
    int ret;

    if (list_empty(&frame_mng->frame_head)) {
        core_log("[test_frame] frame queue is empty, frame_queue_id[%u]\n", frame_mng->frame_queue_id);
        return EC_PARAM_INVALID;
    }

    list_for_each_entry_safe(frame, next, &frame_mng->frame_head, node) {
        ret = frame_mng->cb.run_frame(frame_mng->core_id, frame);
        list_del(&frame->node);
        mm_free(frame->bc_list);
        mm_free(frame);
        if (ret != EC_OK) {
            core_log("[test_frame] run frame failed, frame_queue_id[%u] ret[%d]\n",
                frame_mng->frame_queue_id, ret);
            return ret;
        }
    }
    return EC_OK;
}

int test_frame_enqueue(u8 frame_queue_id, test_frame_s *frame)
{
    test_frame_mng_s *frame_mng = NULL;
    int ret;

    if (frame == NULL) {
        return EC_PARAM_INVALID;
    }
    list_for_each_entry(frame_mng, &g_frame_mng_list, mng_node) {
        if (frame_mng->frame_queue_id == frame_queue_id) {
            list_add_tail(&frame->node, &frame_mng->frame_head);
            ret = test_frame_dequeue(frame_mng);
            if (ret != EC_OK) {
                return ret;
            }
            return EC_OK;
        }
    }
    core_log("[test_frame] enqueue frame_queue_id[%u] not found\n", frame_queue_id);
    return EC_FRAME_Q_ID_INVALID;
}

int test_frame_register(u8 frame_queue_id, u8 core_id, test_frame_callback_s *cb)
{
    test_frame_mng_s *frame_mng = mm_malloc(sizeof(test_frame_mng_s));
    if (frame_mng == NULL) {
        core_log("[test_frame] alloc frame_mng failed\n");
        return EC_ALLOC_FAILED;
    }
    frame_mng->frame_queue_id = frame_queue_id;
    frame_mng->core_id = core_id;
    frame_mng->cb = *cb;
    INIT_LIST_HEAD(&frame_mng->frame_head);
    list_add_tail(&frame_mng->mng_node, &g_frame_mng_list);
    return EC_OK;
}

static void test_frame_free_frame_mng(test_frame_mng_s *frame_mng)
{
    test_frame_s *frame = NULL;
    test_frame_s *next = NULL;
    list_for_each_entry_safe(frame, next, &frame_mng->frame_head, node) {
        list_del(&frame->node);
        mm_free(frame);
    }
}

void test_frame_free_all(void)
{
    test_frame_mng_s *frame_mng = NULL;
    test_frame_mng_s *next = NULL;
    list_for_each_entry_safe(frame_mng, next, &g_frame_mng_list, mng_node) {
        list_del(&frame_mng->mng_node);
        mm_free(frame_mng);
    }
}