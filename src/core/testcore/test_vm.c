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
#include "test_vm.h"
#include "error_code.h"
#include "list.h"
#include "log.h"
#include "mm.h"
#include "test_frame.h"
#include "test_bc.h"

static LIST_HEAD(g_vm_list);

static test_vm_s *test_vm_find_vm(u8 core_id)
{
    test_vm_s *vm = NULL;
    list_for_each_entry(vm, &g_vm_list, node) {
        if (vm->core_id == core_id) {
            return vm;
        }
    }
    return NULL;
}

int test_vm_bc_proc_pre(test_bc_s *bc)
{
    test_vm_s *vm = test_vm_find_vm(bc->core_id);
    int ret;

    log_printf(LOG_DEBUG, "[test_vm] test_vm_bc_proc_pre bc_op: %u\n", bc->op);

    if (vm == NULL) {
        core_log("[test_vm] test_vm_bc_proc_pre core_id[%u] not found\n", bc->core_id);
        return EC_CORE_ID_INVALID;
    }
    if (bc->arg_type == TEST_BC_ARG_TYPE_1_ARG_1_OBJ) {
        ret = test_vm_id_map_get_pair(vm->core_id, bc->obj_vm_id1, &bc->args.bc_1_arg.obj_id);
    } else if (bc->arg_type == TEST_BC_ARG_TYPE_2_ARG_1_OBJ) {
        ret = test_vm_id_map_get_pair(vm->core_id, bc->obj_vm_id1, &bc->args.bc_2_args.obj_id1);
    } else if (bc->arg_type == TEST_BC_ARG_TYPE_2_ARG_2_OBJ) {
        ret = test_vm_id_map_get_pair(vm->core_id, bc->obj_vm_id1, &bc->args.bc_2_args.obj_id1);
        ret |= test_vm_id_map_get_pair(vm->core_id, bc->obj_vm_id2, &bc->args.bc_2_args.obj_id2);
    } else if (bc->arg_type == TEST_BC_ARG_TYPE_3_ARG_3_OBJ) {
        ret = test_vm_id_map_get_pair(vm->core_id, bc->obj_vm_id1, &bc->args.bc_3_args.obj_id1);
        ret |= test_vm_id_map_get_pair(vm->core_id, bc->obj_vm_id2, &bc->args.bc_3_args.t1.obj_id2);
        ret |= test_vm_id_map_get_pair(vm->core_id, bc->obj_vm_id3, &bc->args.bc_3_args.t1.obj_id3);
    } else if (bc->arg_type < TEST_BC_ARG_TYPE_MAX) {
        ret = EC_OK;
    } else {
        ret = EC_OBJ_ARG_TYPE_INVALID;
    }
    if (ret != EC_OK) {
        core_log("[test_vm] test_vm_bc_proc_post failed, bc_op[%u] ret[%d]\n", bc->op, ret);
        return ret;
    }
    return EC_OK;
}

int test_vm_bc_proc_post(test_bc_s *bc)
{
    test_vm_s *vm = test_vm_find_vm(bc->core_id);
    int ret;

    if (vm == NULL) {
        core_log("[test_vm] test_vm_bc_proc_post core_id[%u] not found\n", bc->core_id);
        return EC_CORE_ID_INVALID;
    }
    vm->pc = bc->next_pos;
    if (bc->op == TEST_BC_NEW) {
        ret = test_vm_id_map_add_pair(vm->core_id, bc->obj_vm_id1, bc->bc_new_res.obj_id);
    } else if (bc->op == TEST_BC_DEL) {
        ret = test_vm_id_map_del_pair(vm->core_id, bc->obj_vm_id1, bc->args.bc_1_arg.obj_id);
    } else {
        ret = EC_OK;
    }
    if (ret != EC_OK) {
        core_log("[test_vm] test_vm_bc_proc_post failed, bc_op[%u] ret[%d]\n", bc->op, ret);
        return ret;
    }
    test_bc_debug_print(bc);
    return EC_OK;
}

static int test_vm_run_bc(test_vm_s *vm, test_bc_s *bc)
{
    int ret;
    ret = test_bc_proc(bc, test_vm_bc_proc_pre, test_vm_bc_proc_post);
    if (ret != EC_OK) {
        core_log("[test_vm] run bc failed, ret[%d]\n", ret);
    }
    return ret;
}

static int test_vm_run_bc_list(test_vm_s *vm, test_frame_s *frame)
{
    int i, ret;
    for (i = 0; i < frame->bc_num; i++) {
        ret = test_vm_run_bc(vm, &frame->bc_list[i]);
        if (ret != EC_OK) {
            core_log("[test_vm] run bc list failed, num[%d] ret[%d]\n", i, ret);
            return ret;
        }
    }
    return EC_OK;
}

int test_vm_run_frame(u8 core_id, test_frame_s *frame)
{
    test_vm_s *vm = test_vm_find_vm(core_id);
    if (vm == NULL) {
        core_log("[test_vm] run frame core_id[%u] not found\n", core_id);
        return EC_CORE_ID_INVALID;
    }
    return test_vm_run_bc_list(vm, frame);
}

int test_vm_init(u8 core_id, u8 frame_q_id)
{
    test_frame_callback_s frame_cb = {
        .run_frame = test_vm_run_frame,
    };
    test_vm_s *vm = NULL;
    int ret;

    vm = mm_malloc(sizeof(test_vm_s));
    if (vm == NULL) {
        core_log("[test_vm] init alloc vm failed, core_id[%u]\n", core_id);
        return EC_ALLOC_FAILED;
    }
    vm->core_id = core_id;
    list_add_tail(&vm->node, &g_vm_list);
    ret = test_frame_register(frame_q_id, core_id, &frame_cb);
    if (ret != EC_OK) {
        core_log("[test_vm] init register frame failed, core_id[%u]\n", core_id);
        return ret;
    }
    vm->pc = 0;
    vm->vm_id_map_size = 0;
    INIT_LIST_HEAD(&vm->vm_id_map);
    return EC_OK;
}

int test_vm_add(u8 core_id, u8 frame_q_id)
{
    test_vm_s *vm = test_vm_find_vm(core_id);
    int ret;

    if (vm != NULL) {
        core_log("[test_vm] add core_id[%u] failed, already exist\n", core_id);
        return EC_CORE_ID_INVALID;
    }
    ret = test_vm_init(core_id, frame_q_id);
    if (ret != EC_OK) {
        core_log("[test_vm] add core_id[%u] failed, ret[%d]\n", core_id, ret);
        return ret;
    }
    return EC_OK;
}

int test_vm_free(u8 core_id)
{
    test_vm_s *vm = NULL;
    test_vm_s *next = NULL;
    list_for_each_entry_safe(vm, next, &g_vm_list, node) {
        if (vm->core_id == core_id) {
            list_del(&vm->node);
            mm_free(vm);
            return EC_OK;
        }
    }
    core_log("[test_vm] free core_id[%u] not found\n", core_id);
    return EC_CORE_ID_INVALID;
}

void test_vm_free_all(void)
{
    test_vm_s *vm = NULL;
    test_vm_s *next = NULL;
    list_for_each_entry_safe(vm, next, &g_vm_list, node) {
        list_del(&vm->node);
        mm_free(vm);
    }
}

static test_vm_id_pair_s *test_vm_find_vm_id_map_ele(test_vm_s *vm, u8 obj_vm_id)
{
    test_vm_id_pair_s *ele = NULL;
    list_for_each_entry(ele, &vm->vm_id_map, node) {
        if (ele->obj_vm_id == obj_vm_id) {
            return ele;
        }
    }
    return NULL;
}

int test_vm_id_map_add_pair(u8 core_id, u32 obj_vm_id, u32 obj_id)
{
    test_vm_id_pair_s *ele = NULL;
    test_vm_s *vm = test_vm_find_vm(core_id);
    if (vm == NULL) {
        core_log("[test_vm] add id pair core_id[%u] not found\n", core_id);
        return EC_CORE_ID_INVALID;
    }
    ele = test_vm_find_vm_id_map_ele(vm, obj_vm_id);
    if (ele != NULL) {
        core_log("[test_vm] add id pair obj_vm_id[%u] already exist, obj_id[%u]\n",
            obj_vm_id, obj_id);
        return EC_OBJ_VM_ID_INVALID;
    }
    ele = mm_malloc(sizeof(test_vm_id_pair_s));
    if (ele == NULL) {
        core_log("[test_vm] add id pair alloc failed, core_id[%u] obj_vm_id[%u] obj_id[%u]\n",
            core_id, obj_vm_id, obj_id);
        return EC_ALLOC_FAILED;
    }
    ele->obj_id = obj_id;
    ele->obj_vm_id = obj_vm_id;
    list_add_tail(&ele->node, &vm->vm_id_map);
    vm->vm_id_map_size++;
    return EC_OK;
}

int test_vm_id_map_set_pair(u8 core_id, u32 obj_vm_id, u32 set_obj_id)
{
    test_vm_id_pair_s *ele = NULL;
    test_vm_s *vm = test_vm_find_vm(core_id);
    if (vm == NULL) {
        core_log("[test_vm] set id pair core_id[%u] not found\n", core_id);
        return EC_CORE_ID_INVALID;
    }
    ele = test_vm_find_vm_id_map_ele(vm, obj_vm_id);
    if (ele == NULL) {
        core_log("[test_vm] set id pair obj_vm_id[%u] not found\n", obj_vm_id);
        return EC_OBJ_VM_ID_INVALID;
    }
    ele->obj_id = set_obj_id;
    return EC_OK;
}

int test_vm_id_map_get_pair(u8 core_id, u32 obj_vm_id, u32 *get_obj_id)
{
    test_vm_id_pair_s *ele = NULL;
    test_vm_s *vm = test_vm_find_vm(core_id);
    if (vm == NULL) {
        core_log("[test_vm] get id pair core_id[%u] not found\n", core_id);
        return EC_CORE_ID_INVALID;
    }
    ele = test_vm_find_vm_id_map_ele(vm, obj_vm_id);
    if (ele == NULL) {
        core_log("[test_vm] get id pair obj_vm_id[%u] not found\n", obj_vm_id);
        return EC_OBJ_VM_ID_INVALID;
    }
    *get_obj_id = ele->obj_id;
    return EC_OK;
}

int test_vm_id_map_del_pair(u8 core_id, u32 obj_vm_id, u32 obj_id)
{
    test_vm_id_pair_s *ele = NULL;
    test_vm_id_pair_s *next = NULL;
    test_vm_s *vm = test_vm_find_vm(core_id);
    if (vm == NULL) {
        core_log("[test_vm] del id pair core_id[%u] not found\n", core_id);
        return EC_CORE_ID_INVALID;
    }

    list_for_each_entry_safe(ele, next, &vm->vm_id_map, node) {
        if (ele->obj_id == obj_id && ele->obj_vm_id == obj_vm_id) {
            list_del(&ele->node);
            mm_free(ele);
            vm->vm_id_map_size--;
            return EC_OK;
        }
    }
    core_log("[test_vm] del id pair obj_vm_id[%u] obj_id[%u] not found\n", obj_vm_id, obj_id);
    return EC_OBJ_NOT_FOUND;
}