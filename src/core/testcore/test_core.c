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
#include "test_core.h"
#include "leafpy_cfg.h"
#include "error_code.h"
#include "list.h"
#include "mm.h"
#include "log.h"
#include "test_data.h"

static int test_core_proc_NEW(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_NEW *op_new = &op_info->info.op_new;
    int ret;

    if (op_new->obj_name_len > LEAFPY_MAX_OBJ_NAME_LEN) {
        core_printf("[test_core] NEW obj_name_len[%u] exceed limit\n", op_new->obj_name_len);
        return EC_OBJ_NAME_LEN_EXCEED;
    }
    ret = test_data_obj_new(op_new->obj_type, op_new->obj_subtype, op_new->obj_name,
                            op_new->parent_id, core->global_obj);
    if (ret != EC_OK) {
        core_printf("[test_core] NEW [%s] obj_nameobj_type[%u] obj_subtype[%u] failed\n",
            op_new->obj_name, op_new->obj_type, op_new->obj_subtype);
    }
    return ret;
}

static int test_core_proc_DEL(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_DEL *op_del = &op_info->info.op_del;
    int ret;

    if (op_del->obj_id == GLOBAL_OBJ_ID || op_del->obj_id == ROOT_OBJ_ID) {
        core_printf("[test_core] DEL obj_id[%u] failed, global_obj and root_obj is not deletable\n",
            op_del->obj_id);
        return EC_OBJ_NOT_DELETABLE;
    }
    ret = test_data_obj_del(op_del->obj_id, core->global_obj);
    if (ret != EC_OK) {
        core_printf("[test_core] DEL obj_id[%u] failed\n", op_del->obj_id);
        return ret;
    }
    return EC_OK;
}

static int test_core_proc_FIND(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_FIND *op_find = &op_info->info.op_find;
    test_core_res_FIND *res_find = &op_info->result.res_find;
    int ret;

    ret = test_data_obj_get(op_find->obj_name, op_find->parent_id, core->global_obj,
                            &res_find->obj_id);
    if (ret != EC_OK) {
        core_printf("[test_core] FIND obj_name[%s] failed\n", op_find->obj_name);
        return ret;
    }
    return EC_OK;
}

static int test_core_proc_CALC(test_core_op_info_s *op_info, test_core_s *core)
{
    return EC_OK;
}

static int test_core_proc_PRINT(test_core_op_info_s *op_info, test_core_s *core)
{
    return EC_OK;
}

static int test_core_proc_CALL(test_core_op_info_s *op_info, test_core_s *core)
{
    return EC_OK;
}

struct {
    u8 op;      /* operation: enum test_core_op_e */
    int (*proc)(test_core_op_info_s *op_info, test_core_s *core);
} g_test_core_op_map[] = {
    { TEST_CORE_OP_NEW, test_core_proc_NEW },
    { TEST_CORE_OP_DEL, test_core_proc_DEL },
    { TEST_CORE_OP_FIND, test_core_proc_FIND },
    { TEST_CORE_OP_CALC, test_core_proc_CALC },
    { TEST_CORE_OP_PRINT, test_core_proc_PRINT },
    { TEST_CORE_OP_CALL, test_core_proc_CALL },
};

static int test_core_run_op_proc(test_core_op_info_s *op_info, test_core_s *core)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(g_test_core_op_map); i++) {
        if (g_test_core_op_map[i].op == op_info->op) {
            return g_test_core_op_map[i].proc(op_info, core);
        }
    }
    core_printf("[test_core] op[%u] is unsupported\n", op_info->op);
    return EC_UNSUPPORT_OP;
}

static struct list_head g_core_list;

int test_core_run(u8 core_id, test_core_op_info_s *op_info)
{
    test_core_s *core = NULL;
    int ret;

    if (op_info == NULL) {
        core_printf("[test_core] run op_info is NULL\n");
        return EC_PARAM_INVALID;
    }
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            ret = test_core_run_op_proc(op_info, core);
            if (ret != EC_OK) {
                core_printf("[test_core] run op[%u] failed, core_id[%u] ret[%d]\n",
                    op_info->op, core_id, ret);
            }
            return ret;
        }
    }
    core_printf("[test_core] core_id[%u] invalid, run op[%u] failed\n", core_id);
    return EC_CORE_ID_INVALID;
}

int test_core_init(u8 core_id)
{
    int ret;
    test_core_s *core = mm_malloc(sizeof(*core));

    BUILD_CHECK_OP_INFO_SIZE();

    if (core == NULL) {
        core_printf("[test_core] init alloc core failed, core_id[%u]\n", core_id);
        return EC_ALLOC_FAILED;
    }
    (void)mm_memset_s(core, sizeof(*core), 0, sizeof(*core));
    core->core_id = core_id;
    ret = test_data_init(&core->global_obj);
    if (ret != EC_OK) {
        core_printf("[test_core] init global_obj failed, core_id[%u]\n", core->core_id);
        mm_free(core);
        return ret;
    }

    INIT_LIST_HEAD(&g_core_list);
    list_add_tail(&core->node, &g_core_list);
    return EC_OK;
}

test_core_s *test_core_get_core(u8 core_id)
{
    test_core_s *core = NULL;
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            return core;
        }
    }
    core_printf("[test_core] get core failed, core_id[%u] not exist\n", core_id);
    return NULL;
}

int test_core_free(u8 core_id)
{
    test_core_s *core = NULL;
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            list_del(&core->node);
            mm_free(core);
            return EC_OK;
        }
    }
    core_printf("[test_core] free core failed, core_id[%u] not exist\n", core_id);
    return EC_CORE_ID_INVALID;
}

int test_core_add(u8 core_id)
{
    test_core_s *core = NULL;
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            core_printf("[test_core] core_id[%u] already exist\n", core_id);
            return EC_CORE_ID_INVALID;
        }
    }
    core = mm_malloc(sizeof(test_core_s));
    if (core == NULL) {
        core_printf("[test_core] alloc core failed, core_id[%u]\n", core_id);
        return EC_ALLOC_FAILED;
    }
    core->core_id = core_id;
    list_add_tail(&core->node, &g_core_list);
    return EC_OK;
}

void test_core_print_obj_list(u8 core_id)
{
    test_core_s *core = test_core_get_core(core_id);
    test_data_print_obj_list(core->global_obj);
}