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
        core_log("[test_core] NEW obj_name_len[%u] exceed limit\n", op_new->obj_name_len);
        return EC_OBJ_NAME_LEN_EXCEED;
    }
    ret = test_data_obj_new(op_new->obj_type, op_new->obj_subtype, op_new->obj_name,
                            op_new->parent_id, core->global_obj);
    if (ret != EC_OK) {
        core_log("[test_core] NEW obj_name[%s] obj_type[%u] obj_subtype[%u] failed, ret[%d]\n",
            op_new->obj_name, op_new->obj_type, op_new->obj_subtype, ret);
    }
    return ret;
}

static int test_core_proc_DEL(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_DEL *op_del = &op_info->info.op_del;
    int ret;

    if (op_del->obj_id == GLOBAL_OBJ_ID || op_del->obj_id == ROOT_OBJ_ID) {
        core_log("[test_core] DEL obj_id[%u] failed, global_obj or root_obj is not deletable\n",
            op_del->obj_id);
        return EC_OBJ_NOT_DELETABLE;
    }
    ret = test_data_obj_del(op_del->obj_id, core->global_obj);
    if (ret != EC_OK) {
        core_log("[test_core] DEL obj_id[%u] failed, ret[%d]\n", op_del->obj_id, ret);
        return ret;
    }
    return EC_OK;
}

static int test_core_proc_FIND(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_FIND *op_find = &op_info->info.op_find;
    test_core_res_FIND *res_find = &op_info->result.res_find;
    int ret;

    ret = test_data_obj_get_id_by_name(op_find->obj_name, op_find->parent_id, core->global_obj,
                                       &res_find->obj_id);
    if (ret != EC_OK) {
        core_log("[test_core] FIND obj_name[%s] failed, ret[%d]\n", op_find->obj_name, ret);
        return ret;
    }
    return EC_OK;
}

static int test_core_proc_SET(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_SET *op_set = &op_info->info.op_set;
    obj_op_info_s info = {
        .op = OBJ_OP_SET,
        .one_obj = {
            .obj_type = op_set->obj_type,
            .obj_subtype = op_set->obj_subtype,
            .obj_id = op_set->obj_id,
        },
        .ret_val = op_set->obj_val,
        .global_obj = core->global_obj,
    };
    int ret;
    ret = test_data_obj_op_proc(&info);
    if (ret != EC_OK) {
        core_log("[test_core] SET obj_id[%u] failed, ret[%d]\n", op_set->obj_id, ret);
        return ret;
    }
    return EC_OK;
}

static int test_core_proc_GET(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_GET *op_get = &op_info->info.op_get;
    test_core_res_GET *res_get = &op_info->result.res_get;
    obj_op_info_s info = {
        .op = OBJ_OP_GET,
        .one_obj = {
            .obj_type = op_get->obj_type,
            .obj_subtype = op_get->obj_subtype,
            .obj_id = op_get->obj_id,
        },
        .ret_val = res_get->obj_val,
        .global_obj = core->global_obj,
    };
    int ret;
    ret = test_data_obj_op_proc(&info);
    if (ret != EC_OK) {
        core_log("[test_core] GET obj_id[%u] failed, ret[%d]\n", op_get->obj_id, ret);
        return ret;
    }
    return EC_OK;
}

static int test_core_proc_CVT(test_core_op_info_s *op_info, test_core_s *core)
{
    test_core_op_CVT *op_cvt = &op_info->info.op_cvt;
    test_core_res_CVT *res_cvt = &op_info->result.res_cvt;
    test_core_op_info_s tmp_info;
    union {
        s64 int_val;
        f64 float_val;
    } res_val = {0};
    u8 tgt_name_len = 0;
    char *tgt_name = NULL;
    u32 tgt_id = 0;
    int ret = EC_OK;

    if (op_cvt->tgt_type == op_cvt->cur_type) {
        goto out;
    }

    if (op_cvt->tgt_type != OBJ_TYPE_NUMBER) {
        ret = EC_MAY_SUPPORT_LATER;
    }

    // GET
    tmp_info.op = TEST_CORE_OP_GET;
    tmp_info.info.op_get.obj_type = op_cvt->cur_type;
    tmp_info.info.op_get.obj_subtype = op_cvt->cur_subtype;
    tmp_info.info.op_get.obj_id = op_cvt->cur_obj_id;
    tmp_info.result.res_get.obj_val = &res_val;
    ret = test_core_proc_GET(&tmp_info, core);
    if (ret != EC_OK) {
        goto out;
    }
    // get target obj name and name len
    ret = test_data_obj_get_name_len_by_id(op_cvt->cur_obj_id, core->global_obj, &tgt_name_len);
    if (ret != EC_OK) {
        goto out;
    }
    tgt_name = mm_malloc(tgt_name_len);
    if (tgt_name == NULL) {
        ret = EC_ALLOC_FAILED;
        goto out;  // free is not needed
    }
    ret = test_data_obj_get_name_by_id(op_cvt->cur_obj_id, tgt_name_len, core->global_obj, &tgt_name);
    if (ret != EC_OK) {
        goto out;
    }
    // FIND
    tmp_info.op = TEST_CORE_OP_FIND;
    tmp_info.info.op_find.obj_name = tgt_name;
    tmp_info.info.op_find.parent_id = op_cvt->tgt_parent_id;
    ret = test_core_proc_FIND(&tmp_info, core);
    if (ret != EC_OK) {
        goto out;
    }
    tgt_id = tmp_info.result.res_find.obj_id;
    // NEW
    tmp_info.op = TEST_CORE_OP_NEW;
    tmp_info.info.op_new.obj_type = op_cvt->tgt_type;
    tmp_info.info.op_new.obj_subtype = op_cvt->tgt_subtype;
    tmp_info.info.op_new.obj_name_len = 0;  // promise obj_name_len is legal
    tmp_info.info.op_new.parent_id = op_cvt->tgt_parent_id;
    tmp_info.info.op_new.obj_name = tgt_name;
    ret = test_core_proc_NEW(&tmp_info, core);
    if (ret != EC_OK) {
        goto out;
    }
    // SET
    tmp_info.op = TEST_CORE_OP_SET;
    tmp_info.info.op_set.obj_type = op_cvt->tgt_type;
    tmp_info.info.op_set.obj_subtype = op_cvt->tgt_subtype;
    tmp_info.info.op_set.obj_id = tgt_id;
    switch (op_cvt->tgt_subtype) {
        case NUM_TYPE_INT:
            res_val.int_val = (s64)res_val.float_val;
            break;
        case NUM_TYPE_FLOAT:
            res_val.float_val = (f64)res_val.int_val;
            break;
        default:
            ret = EC_PARAM_INVALID;
            goto out;
    }
    tmp_info.info.op_set.obj_val = &res_val;
    ret = test_core_proc_SET(&tmp_info, core);
    if (ret != EC_OK) {
        goto out;
    }
out:
    if (ret != EC_OK) {
        core_log("[test_core] CVT obj_id[%u] failed, ret[%d]\n", op_cvt->cur_obj_id, ret);
    }
    return ret;
}

static int test_core_proc_CALC(test_core_op_info_s *op_info, test_core_s *core)
{
    static u8 calc_obj_op_map[] = {
        [CALC_OP_ADD] = OBJ_OP_ADD,
        [CALC_OP_SUB] = OBJ_OP_SUB,
        [CALC_OP_MUL] = OBJ_OP_MUL,
        [CALC_OP_DIV] = OBJ_OP_DIV
    };
    test_core_op_CALC *op_calc = &op_info->info.op_calc;
    test_core_res_CALC *res_calc = &op_info->result.res_calc;
    obj_op_info_s info = {
        .two_obj = {
            .obj1_type = op_calc->obj1_type,
            .obj1_subtype = op_calc->obj1_subtype,
            .obj1_id = op_calc->obj1_id,
            .obj2_type = op_calc->obj2_type,
            .obj2_subtype = op_calc->obj2_subtype,
            .obj2_id = op_calc->obj2_id,
        },
        .ret_val_len = op_calc->val_len,
        .ret_val = res_calc->obj_val,
        .global_obj = core->global_obj,
    };
    int ret;

    if (info.op >= CALC_OP_NUM) {
        core_log("[test_core] CALC unsupport op[%d]\n", op_calc->op);
        return EC_UNSUPPORT_OP;
    }

    info.op = calc_obj_op_map[op_calc->op];
    ret = test_data_obj_op_proc(&info);
    if (ret != EC_OK) {
        core_log("[test_core] CALC op[%u] obj1_id[%u] obj2_id[%u] failed, ret[%d]",
            op_calc->op, op_calc->obj1_id, op_calc->obj2_id, ret);
        return ret;
    }
    return EC_OK;
}

static int test_core_proc_LOGIC(test_core_op_info_s *op_info, test_core_s *core)
{
    static u8 logic_obj_op_map[] = { 
        [LOGIC_OP_LOGIC_NOT] = OBJ_OP_LOGIC_NOT,
        [LOGIC_OP_EQ]        = OBJ_OP_EQ,
        [LOGIC_OP_NE]        = OBJ_OP_NE,
        [LOGIC_OP_GE]        = OBJ_OP_GE,
        [LOGIC_OP_GT]        = OBJ_OP_GT,
        [LOGIC_OP_LE]        = OBJ_OP_LE,
        [LOGIC_OP_LT]        = OBJ_OP_LT,
        [LOGIC_OP_LOGIC_AND] = OBJ_OP_LOGIC_AND,
        [LOGIC_OP_LOGIC_OR]  = OBJ_OP_LOGIC_OR
    };
    test_core_op_LOGIC *op_logic = &op_info->info.op_logic;
    test_core_res_LOGIC *res_logic = &op_info->result.res_logic;
    obj_op_info_s info = {
        .ret_val_len = op_logic->val_len,
        .ret_val = &res_logic->ret_bool,
        .global_obj = core->global_obj,
    };
    int ret;

    if (info.op >= LOGIC_OP_NUM) {
        core_log("[test_core] LOGIC unsupport op[%d]\n", op_logic->op);
        return EC_UNSUPPORT_OP;
    }
    info.op = logic_obj_op_map[op_logic->op];
    if (info.op < ONE_LOGIC_OP_MAX) {
        info.one_obj.obj_type = op_logic->obj1_type;
        info.one_obj.obj_subtype = op_logic->obj1_subtype;
        info.one_obj.obj_id = op_logic->obj1_id;
    } else {
        info.two_obj.obj1_type = op_logic->obj1_type;
        info.two_obj.obj1_subtype = op_logic->obj1_subtype;
        info.two_obj.obj1_id = op_logic->obj1_id;
        info.two_obj.obj2_type = op_logic->obj2_type;
        info.two_obj.obj2_subtype = op_logic->obj2_subtype;
        info.two_obj.obj2_id = op_logic->obj2_id;
    }

    ret = test_data_obj_op_proc(&info);
    if (ret != EC_OK) {
        core_log("[test_core] LOGIC op[%u] obj1_id[%u] obj2_id[%u] failed, ret[%d]",
            op_logic->op, op_logic->obj1_id, op_logic->obj2_id, ret);
    }
    return EC_OK;
}

static int test_core_proc_REG(test_core_op_info_s *op_info, test_core_s *core)
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
    { TEST_CORE_OP_SET, test_core_proc_SET },
    { TEST_CORE_OP_GET, test_core_proc_GET },
    { TEST_CORE_OP_CVT, test_core_proc_GET },
    { TEST_CORE_OP_CALC, test_core_proc_CALC },
    { TEST_CORE_OP_LOGIC, test_core_proc_LOGIC },
    { TEST_CORE_OP_REG, test_core_proc_REG },
    { TEST_CORE_OP_CALL, test_core_proc_CALL },
};

#define BUILD_CHECK_CORE_OP_MAP_SIZE() BUILD_BUG_ON(ARRAY_SIZE(g_test_core_op_map) != TEST_CORE_OP_NUM)

static int test_core_run_op_proc(test_core_op_info_s *op_info, test_core_s *core)
{
    BUILD_CHECK_CORE_OP_MAP_SIZE();

    if (op_info->op < TEST_CORE_OP_NUM) {
        return g_test_core_op_map[op_info->op].proc(op_info, core);
    } else {
        core_log("[test_core] op[%u] is unsupported\n", op_info->op);
        return EC_UNSUPPORT_OP;
    }
}

static struct list_head g_core_list;

int test_core_run(u8 core_id, test_core_op_info_s *op_info)
{
    test_core_s *core = NULL;
    int ret;

    if (op_info == NULL) {
        core_log("[test_core] run op_info is NULL\n");
        return EC_PARAM_INVALID;
    }
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            ret = test_core_run_op_proc(op_info, core);
            if (ret != EC_OK) {
                core_log("[test_core] run op[%u] failed, core_id[%u] ret[%d]\n",
                    op_info->op, core_id, ret);
            }
            return ret;
        }
    }
    core_log("[test_core] core_id[%u] invalid, run op[%u] failed\n", core_id);
    return EC_CORE_ID_INVALID;
}

int test_core_init(u8 core_id)
{
    int ret;
    test_core_s *core = mm_malloc(sizeof(*core));

    BUILD_CHECK_CORE_OP_INFO_SIZE();

    if (core == NULL) {
        core_log("[test_core] init alloc core failed, core_id[%u]\n", core_id);
        return EC_ALLOC_FAILED;
    }
    (void)mm_memset_s(core, sizeof(*core), 0, sizeof(*core));
    core->core_id = core_id;

    ret = test_data_init(&core->global_obj);
    if (ret != EC_OK) {
        core_log("[test_core] init global_obj failed, core_id[%u]\n", core->core_id);
        mm_free(core);
        return ret;
    }

    INIT_LIST_HEAD(&g_core_list);
    list_add_tail(&core->node, &g_core_list);
    return EC_OK;
}

int test_core_register_cfunc(register_cfunc_s *reg_cfunc)
{
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
    core_log("[test_core] get core failed, core_id[%u] not exist\n", core_id);
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
    core_log("[test_core] free core failed, core_id[%u] not exist\n", core_id);
    return EC_CORE_ID_INVALID;
}

int test_core_add(u8 core_id)
{
    test_core_s *core = NULL;
    list_for_each_entry(core, &g_core_list, node) {
        if (core->core_id == core_id) {
            core_log("[test_core] core_id[%u] already exist\n", core_id);
            return EC_CORE_ID_INVALID;
        }
    }
    core = mm_malloc(sizeof(test_core_s));
    if (core == NULL) {
        core_log("[test_core] alloc core failed, core_id[%u]\n", core_id);
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