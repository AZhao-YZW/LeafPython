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
#ifndef _TEST_VM_H_
#define _TEST_VM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "list.h"
#include "test_frame.h"

/**
 * obj_vm_id is used by the VM to manage the object.
 * one obj_vm_id is mapped to one or more obj_id.
 */
typedef struct {
    struct list_head node;
    u32 obj_vm_id;
    u32 obj_id;
} test_vm_id_pair_s;

typedef struct {
    struct list_head node;
    struct list_head vm_id_map;
    u8 core_id;
    u8 rsv[3];
    u32 vm_id_map_size;
    u64 pc;
} test_vm_s;

int test_vm_run_frame(u8 core_id, test_frame_s *frame);
int test_vm_init(u8 core_id, u8 frame_q_id);
int test_vm_add(u8 core_id, u8 frame_q_id);
int test_vm_free(u8 core_id);
void test_vm_free_all(void);
int test_vm_id_map_add_pair(u8 core_id, u32 obj_vm_id, u32 obj_id);
int test_vm_id_map_set_pair(u8 core_id, u32 obj_vm_id, u32 set_obj_id);
int test_vm_id_map_get_pair(u8 core_id, u32 obj_vm_id, u32 *get_obj_id);
int test_vm_id_map_del_pair(u8 core_id, u32 obj_vm_id, u32 obj_id);

#ifdef __cplusplus
}
#endif
#endif