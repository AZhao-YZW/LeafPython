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
#include "error_code.h"
#include "list.h"
#include "mm.h"
#include "log.h"
#include "test_data.h"

static int test_core_proc_NEW(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_NEW *op_new = &op_info->info.op_new;
    int ret;
    ret = test_data_obj_new(op_new->obj_type, op_new->obj_name, op_new->parent_id, core->global_obj);
    if (ret != EC_OK) {
        core_printf("[test_core] NEW obj_type[%u] failed\n", op_new->obj_type);
    }
    return ret;
}

static int test_core_proc_DEL(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_DEL *op_del = &op_info->info.op_del;
    int ret;
    ret = test_data_obj_del(op_del->obj_id, core->global_obj);
    if (ret != EC_OK) {
        core_printf("[test_core] DEL obj_id[%u] failed\n", op_del->obj_id);
    }
    return EC_OK;
}

static int test_core_proc_ADD(test_core_op_info_s *op_info, test_core_s *core)
{
    return EC_OK;
}
static int test_core_proc_SUB(test_core_op_info_s *op_info, test_core_s *core)
{
    return EC_OK;
}

static int test_core_proc_MUL(test_core_op_info_s *op_info, test_core_s *core)
{
    return EC_OK;
}

static int test_core_proc_DIV(test_core_op_info_s *op_info, test_core_s *core)
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
    { TEST_CORE_OP_ADD, test_core_proc_ADD },
    { TEST_CORE_OP_SUB, test_core_proc_SUB },
    { TEST_CORE_OP_MUL, test_core_proc_MUL },
    { TEST_CORE_OP_DIV, test_core_proc_DIV },
    { TEST_CORE_OP_PRINT, test_core_proc_PRINT },
    { TEST_CORE_OP_CALL, test_core_proc_CALL },
};

int test_core_run_op_proc(test_core_op_info_s *op_info, test_core_s *core)
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
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            return test_core_run_op_proc(op_info, core);
        }
    }
    core_printf("[test_core] core_id[%u] invalid, run op[%u] failed\n", core_id);
    return EC_INVALID_CORE_ID;
}

int test_core_init(u8 core_id)
{
    int ret;
    test_core_s *core = mm_malloc(sizeof(*core));
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
    return EC_INVALID_CORE_ID;
}

int test_core_add(u8 core_id)
{
    test_core_s *core = NULL;
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            core_printf("[test_core] core_id[%u] already exist\n", core_id);
            return EC_INVALID_CORE_ID;
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