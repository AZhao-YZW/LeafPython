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
#include "test_bc.h"
#include "error_code.h"
#include "log.h"
#include "test_core.h"

#define TEST_BC_CORE_OP_NEW_FILL(bc, tn, op_info) \
    test_core_op_new_fill(bc->args.obj_type##tn, bc->args.obj_subtype##tn, \
        bc->parent_id, bc->args.bc_new.obj_name, &op_info)

#define TEST_BC_CORE_OP_DEL_FILL(bc, op_info) \
    test_core_op_del_fill(bc->args.bc_del.obj_id, &op_info)

#define TEST_BC_CORE_OP_SET_FILL(obj_id, set_val, op_info) \
    test_core_op_set_fill(obj_id, set_val, &op_info)

#define TEST_BC_CORE_OP_GET_FILL(bc, tn, get_val, op_info) \
    test_core_op_get_fill(bc->args.bc_mov.obj_id##tn, get_val, &op_info)

#define TEST_BC_CORE_OP_CACL_FILL(bc, tn1, tn2, calc_op, res_val, op_info) \
    test_core_op_cacl_fill(calc_op, bc->args.bc_add_sub_mul_div.t1.obj_id##tn1, \
        bc->args.bc_add_sub_mul_div.t1.obj_id##tn2, res_val, bc->args.bc_add_sub_mul_div.val_len, &op_info)

#define TEST_BC_CORE_OP_LOGIC_FILL(bc, tn1, tn2, logic_op, res_val, op_info) \
    test_core_op_logic_fill(logic_op, bc->args.obj_type##tn1, bc->args.obj_subtype##tn1, \
        bc->args.bc_cmp.t1.obj_id##tn1, bc->args.obj_type##tn2, bc->args.obj_subtype##tn2, \
        bc->args.bc_cmp.t1.obj_id##tn2, res_val, bc->args.bc_cmp.val_len, &op_info)

int test_bc_proc_NOP(test_bc_s *bc)
{
    return EC_OK;
}

int test_bc_proc_NEW(test_bc_s *bc)
{
    test_core_op_info_s op_info = {0};
    int ret;

    // NEW <obj_name> <type>
    TEST_BC_CORE_OP_NEW_FILL(bc, 1, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        return ret;
    }
    bc->bc_new_res.obj_id = op_info.result.res_new.obj_id;
    return EC_OK;
}

int test_bc_proc_DEL(test_bc_s *bc)
{
    test_core_op_info_s op_info = {0};

    // DEL <obj_id>
    TEST_BC_CORE_OP_DEL_FILL(bc, op_info);
    return test_core_run(bc->core_id, &op_info);
}

int test_bc_proc_MOV_obj(test_bc_s *bc)
{
    test_core_op_info_s op_info = {0};
    s64 val = {0};
    int ret;

    // MOV <obj_id1> <obj_id2>
    // get obj_id2 value
    TEST_BC_CORE_OP_GET_FILL(bc, 2, &val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] MOV: get obj_id2 value error, ret[%d]\n", ret);
        return ret;
    }
    // set obj_id1 value
    TEST_BC_CORE_OP_SET_FILL(bc->args.bc_mov.obj_id1, &val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] MOV: set obj_id1 value error, ret[%d]\n", ret);
        return ret;
    }
    return EC_OK;
}

int test_bc_proc_MOV_val(test_bc_s *bc)
{
    test_core_op_info_s op_info = {0};
    int ret;

    // MOV <obj_id> <value>
    // set obj_id1 value
    TEST_BC_CORE_OP_SET_FILL(bc->args.bc_mov.obj_id1, bc->args.bc_mov.val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] MOV: set obj_id1 value error, ret[%d]\n", ret);
        return ret;
    }
    return EC_OK;
}

int test_bc_proc_MOV(test_bc_s *bc)
{
    if (bc->sub_op == TEST_BC_MOV_OBJ) {
        return test_bc_proc_MOV_obj(bc);
    } else {
        return test_bc_proc_MOV_val(bc);
    }
}

static int test_bc_proc_INC_DEC(test_bc_s *bc, bool is_inc)
{
    test_core_op_info_s op_info = {0};
    union {
        s64 int_val;
        f64 float_val;
    } val = {0};
    int ret;

    if (bc->args.obj_type1 != OBJ_TYPE_NUMBER ||
        (bc->args.obj_subtype1 != NUM_TYPE_INT && bc->args.obj_subtype1 != NUM_TYPE_FLOAT)) {
        log_printf(LOG_WARN, "[test_bc] INC_DEC[%u]: type[%u,%u] unsupport\n",
            is_inc, bc->args.obj_type1, bc->args.obj_subtype1);
        return EC_UNSUPPORT_OP;
    }

    // INC/DEC <obj_id>
    // get obj_id value
    TEST_BC_CORE_OP_GET_FILL(bc, 1, &val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] INC_DEC[%u]: get obj_id value error, ret[%d]\n",
            is_inc, ret);
        return ret;
    }
    // inc/dec obj_id value
    if (bc->args.obj_subtype1 == NUM_TYPE_INT) {
        val.int_val += is_inc ? 1 : -1;
    } else {
        val.float_val += is_inc ? 1.0 : -1.0;
    }
    // set obj_id value
    TEST_BC_CORE_OP_SET_FILL(bc->args.bc_inc_dec.obj_id, &val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] INC_DEC[%u]: set obj_id value error, ret[%d]\n",
            is_inc, ret);
        return ret;
    }
    return EC_OK;
}

int test_bc_proc_INC(test_bc_s *bc)
{
    return test_bc_proc_INC_DEC(bc, true);
}

int test_bc_proc_DEC(test_bc_s *bc)
{
    return test_bc_proc_INC_DEC(bc, false);
}

static int test_bc_proc_CALC(test_bc_s *bc, enum test_core_calc_op_e calc_op)
{
    test_core_op_info_s op_info = {0};
    s64 val = {0};
    int ret;

    // [CALC] <obj_id1> <obj_id2> <obj_id3>
    // calc obj_id2 and obj_id3, get result
    TEST_BC_CORE_OP_CACL_FILL(bc, 2, 3, calc_op, &val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] CACL: calc_op[%u] obj_id2 and obj_id3 error, ret[%d]\n",
            calc_op, ret);
        return ret;
    }
    // set obj_id1 value
    TEST_BC_CORE_OP_SET_FILL(bc->args.bc_add_sub_mul_div.obj_id1, &val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] CACL: calc_op[%u] set obj_id1 value error, ret[%d]\n",
            calc_op, ret);
        return ret;
    }
    return EC_OK;
}

int test_bc_proc_ADD(test_bc_s *bc)
{
    return test_bc_proc_CALC(bc, CALC_OP_ADD);
}

int test_bc_proc_SUB(test_bc_s *bc)
{
    return test_bc_proc_CALC(bc, CALC_OP_SUB);
}

int test_bc_proc_MUL(test_bc_s *bc)
{
    return test_bc_proc_CALC(bc, CALC_OP_MUL);
}

int test_bc_proc_DIV(test_bc_s *bc)
{
    return test_bc_proc_CALC(bc, CALC_OP_DIV);
}

// TODO: confirm whether need CMP bytecode
int test_bc_proc_CMP(test_bc_s *bc)
{
    test_core_op_info_s op_info = {0};
    s64 cmp_res = 0, val;
    int ret;

    if (bc->args.obj_type1 != OBJ_TYPE_NUMBER || bc->args.obj_subtype1 != NUM_TYPE_INT) {
        log_printf(LOG_WARN, "[test_bc] CMP: type[%u,%u] unsupport\n",
            bc->args.obj_type1, bc->args.obj_subtype1);
        return EC_UNSUPPORT_OP;
    }

    // CMP <obj_id1> <obj_id2> <obj_id3>
    // compare obj_id2 and obj_id3, if equal, set obj_id1 value to 0
    TEST_BC_CORE_OP_LOGIC_FILL(bc, 2, 3, LOGIC_OP_EQ, &cmp_res, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] CMP: EQ compare error, ret[%d]\n", ret);
        return ret;
    }
    if (cmp_res == true) {
        val = 0;
        goto set_val_out;
    }
    // else if obj_id2 > obj_id3, set obj_id1 value to 1
    TEST_BC_CORE_OP_LOGIC_FILL(bc, 2, 3, LOGIC_OP_GT, &cmp_res, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] CMP: GT compare error, ret[%d]\n", ret);
        return ret;
    }
    if (cmp_res == true) {
        val = 1;
        goto set_val_out;
    }
    // else, set obj_id1 value to -1
    val = -1;

set_val_out:
    TEST_BC_CORE_OP_SET_FILL(bc->args.bc_cmp.obj_id1, &val, op_info);
    ret = test_core_run(bc->core_id, &op_info);
    if (ret != EC_OK) {
        log_printf(LOG_WARN, "[test_bc] CMP: set obj_id1 value[%llu] error, ret[%d]\n",
            val, ret);
        return ret;
    }
    return EC_OK;
}

int test_bc_proc_JZ(test_bc_s *bc)
{
    if (bc->args.arg[0] == 0) {
        bc->next_pos = bc->args.arg[0];
    }
}

int test_bc_proc_JNZ(test_bc_s *bc)
{
    if (bc->args.arg[0] != 0) {
        bc->next_pos = bc->args.arg[1];
    }
}

int test_bc_proc_JMP(test_bc_s *bc)
{
    bc->next_pos = bc->args.arg[0];
}

int test_bc_proc_PRINT(test_bc_s *bc)
{
}

int test_bc_proc_EXIT(test_bc_s *bc)
{
}

int test_bc_proc_CALL(test_bc_s *bc)
{   
}

int test_bc_proc_RET(test_bc_s *bc)
{   
}

int test_bc_proc_FUNC(test_bc_s *bc)
{   
}

struct {
    enum test_bc_op_e op;
    int (*proc)(test_bc_s *bc);
} g_test_bc_map[] = {
    { TEST_BC_NOP, test_bc_proc_NOP },
    { TEST_BC_NEW, test_bc_proc_NEW },
    { TEST_BC_DEL, test_bc_proc_DEL },
    { TEST_BC_MOV, test_bc_proc_MOV },
    { TEST_BC_INC, test_bc_proc_INC },
    { TEST_BC_DEC, test_bc_proc_DEC },
    { TEST_BC_ADD, test_bc_proc_ADD },
    { TEST_BC_SUB, test_bc_proc_SUB },
    { TEST_BC_MUL, test_bc_proc_MUL },
    { TEST_BC_DIV, test_bc_proc_DIV },
    { TEST_BC_CMP, test_bc_proc_CMP },
    { TEST_BC_JZ, test_bc_proc_JZ },
    { TEST_BC_JNZ, test_bc_proc_JNZ },
    { TEST_BC_JMP, test_bc_proc_JMP },
    { TEST_BC_PRINT, test_bc_proc_PRINT },
    { TEST_BC_EXIT, test_bc_proc_EXIT },
    { TEST_BC_CALL, test_bc_proc_CALL },
    { TEST_BC_RET, test_bc_proc_RET },
    { TEST_BC_FUNC, test_bc_proc_FUNC },
};

int test_bc_proc(test_bc_s *bc, test_bc_cb_t cb)
{
    BUILD_CHECK_TEST_BC_ARGS_SIZE();

    int i, ret;
    for (i = 0; i < ARRAY_SIZE(g_test_bc_map); i++) {
        if (g_test_bc_map[i].op == bc->op) {
            ret = g_test_bc_map[i].proc(bc);
            if (ret != EC_OK) {
                core_log("[test_bc] test_bc_proc error, bc_op[%u] sub_op[%u] ret[%d]\n",
                    bc->op, bc->sub_op, ret);
                return ret;
            }
            ret = cb(bc);
            if (ret != 0) {
                core_log("[test_bc] test_bc_proc cb ret[%d]\n", ret);
            }
            return ret;
        }
    }
    log_printf(LOG_ERROR, "unknown op: %u\n", bc->op);
    return EC_UNSUPPORT_OP;
}