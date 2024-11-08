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
#include "test_data.h"
#include "error_code.h"
#include "mm.h"
#include "log.h"
#include "list.h"
#include "libstr.h"

static obj_base_attr_s *test_data_find_obj_by_id(u32 obj_id, global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr = NULL;

    if (global_obj == NULL) {
        core_log("[test_data] global_obj is NULL\n");
        return NULL;
    }

    if (obj_id == GLOBAL_OBJ_ID) {
        return &global_obj->obj_attr;
    }

    list_for_each_entry(obj_attr, &global_obj->obj_attr.node, node) {
        if (obj_attr->obj_id == obj_id) {
            return obj_attr;
        }
    }
    return NULL;
}

static obj_base_attr_s *test_data_find_obj_by_name(const char *obj_name, u32 parent_id,
                                                   global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr = NULL;

    if (global_obj == NULL) {
        core_log("[test_data] global_obj is NULL\n");
        return NULL;
    }

    if (parent_id == GLOBAL_OBJ_ID && !libstr_strcmp(obj_name, GLOBAL_OBJ_NAME)) {
        log_printf(LOG_DEBUG, "[test_data] find obj_name[%s]\n", obj_name);
        return &global_obj->obj_attr;
    }
    list_for_each_entry(obj_attr, &global_obj->obj_attr.node, node) {
        log_printf(LOG_DEBUG, "[test_data] obj_attr->obj_name[%s]\n", obj_attr->obj_name);
        if (obj_attr->parent_id == parent_id && !libstr_strcmp(obj_name, obj_attr->obj_name)) {
            log_printf(LOG_DEBUG, "[test_data] find obj_name[%s]\n", obj_name);
            return obj_attr;
        }
    }
    return NULL;
}

/*****************************************************************************
 *                          all object operation
 *****************************************************************************/
DEFINE_OBJ_ALL_OP_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_INT, offsetof(int_obj_s, val), s64)
DEFINE_OBJ_ALL_OP_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT, offsetof(float_obj_s, val), f64)
DEFINE_OBJ_ALL_OP_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_BOOL, offsetof(bool_obj_s, val), u8)

/*****************************************************************************
 *                          one object operation
 *****************************************************************************/
DEFINE_OBJ_SET_FUNC(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE, offsetof(string_obj_s, val), char *)
DEFINE_OBJ_GET_FUNC(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE, offsetof(string_obj_s, val), char *)

DEFINE_ONE_OBJ_NA_FUNC(logic_not, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)

static int test_data_get_one_obj_type_op(u8 t, u8 st, one_obj_type_op_s **type_op)
{
    static one_obj_type_op_s type_op_map[] = {
        ONE_OBJ_OP_ELE(OBJ_TYPE_NUMBER, NUM_TYPE_INT),
        ONE_OBJ_OP_ELE(OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT),
        ONE_OBJ_OP_ELE(OBJ_TYPE_NUMBER, NUM_TYPE_BOOL),
        ONE_OBJ_OP_ELE(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE),
    };
    u16 obj_ut = OBJ_UNION_TYPE(t, st);
    int i;
    for (i = 0; i < ARRAY_SIZE(type_op_map); i++) {
        if (obj_ut == type_op_map[i].obj_ut) {
            *type_op = &type_op_map[i];
            return EC_OK;
        }
    }
    core_log("[test_data] unsupport type op: [%d, %d]\n", t, st);
    return EC_OBJ_TYPE_INVALID;
}

/*****************************************************************************
 *                          two object operation
 *****************************************************************************/
DEFINE_TWO_OBJ_NA_FUNC(sub, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)
DEFINE_TWO_OBJ_NA_FUNC(mul, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)
DEFINE_TWO_OBJ_NA_FUNC(div, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)
DEFINE_TWO_OBJ_NA_FUNC(logic_and, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)
DEFINE_TWO_OBJ_NA_FUNC(logic_or, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)
static int OBJ_FUNC_NAME(add, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)(void *obj1, void *obj2, void *val, u32 val_len)
{
    int ret;
    ret = libstr_strcpy_s((char *)val, val_len, ((string_obj_s *)obj1)->val);
    if (ret != EC_OK) {
        return ret;
    }
    ret = libstr_strcat_s((char *)val, val_len, ((string_obj_s *)obj2)->val);
    if (ret != EC_OK) {
        return ret;
    }
    return EC_OK;
}

static int OBJ_FUNC_NAME(eq, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)(void *obj1, void *obj2, void *val, u32 val_len)
{
    *(bool *)val = libstr_strcmp(((string_obj_s *)obj1)->val, ((string_obj_s *)obj2)->val) != 0;
    return EC_OK;
}

static int OBJ_FUNC_NAME(ne, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)(void *obj1, void *obj2, void *val, u32 val_len)
{
    *(bool *)val = libstr_strcmp(((string_obj_s *)obj1)->val, ((string_obj_s *)obj2)->val) == 0;
    return EC_OK;
}

static int OBJ_FUNC_NAME(ge, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)(void *obj1, void *obj2, void *val, u32 val_len)
{
    *(bool *)val = libstr_strcmp(((string_obj_s *)obj1)->val, ((string_obj_s *)obj2)->val) >= 0;
    return EC_OK;
}

static int OBJ_FUNC_NAME(gt, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)(void *obj1, void *obj2, void *val, u32 val_len)
{
    *(bool *)val = libstr_strcmp(((string_obj_s *)obj1)->val, ((string_obj_s *)obj2)->val) > 0;
    return EC_OK;
}

static int OBJ_FUNC_NAME(le, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)(void *obj1, void *obj2, void *val, u32 val_len)
{
    *(bool *)val = libstr_strcmp(((string_obj_s *)obj1)->val, ((string_obj_s *)obj2)->val) <= 0;
    return EC_OK;
}

static int OBJ_FUNC_NAME(lt, OBJ_TYPE_STRING, NO_OBJ_SUBTYPE)(void *obj1, void *obj2, void *val, u32 val_len)
{
    *(bool *)val = libstr_strcmp(((string_obj_s *)obj1)->val, ((string_obj_s *)obj2)->val) < 0;
    return EC_OK;
}

static int test_data_get_two_obj_type_op(u8 t1, u8 st1, u8 t2, u8 st2, two_obj_type_op_s **type_op)
{
    static two_obj_type_op_s type_op_map[] = {
        TWO_OBJ_OP_ELE(OBJ_TYPE_NUMBER, NUM_TYPE_INT),
        TWO_OBJ_OP_ELE(OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT),
        TWO_OBJ_OP_ELE(OBJ_TYPE_NUMBER, NUM_TYPE_BOOL),
        TWO_OBJ_OP_ELE(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE),
    };
    u16 obj1_ut = OBJ_UNION_TYPE(t1, st1);
    u16 obj2_ut = OBJ_UNION_TYPE(t2, st2);
    int i;
    for (i = 0; i < ARRAY_SIZE(type_op_map); i++) {
        if (obj1_ut == type_op_map[i].obj1_ut && obj2_ut == type_op_map[i].obj2_ut) {
            *type_op = &type_op_map[i];
            return EC_OK;
        }
    }
    core_log("[test_data] unsupport type op: [%d,%d] [%d,%d]\n", t1, st1, t2, st2);
    return EC_OBJ_TYPE_INVALID;
}

static int test_data_one_obj_op_proc(obj_op_info_s *info)
{
    u8 t = info->one_obj.obj_type;
    u8 st = info->one_obj.obj_subtype;
    u32 obj_id = info->one_obj.obj_id;
    obj_base_attr_s *obj_attr = test_data_find_obj_by_id(obj_id, info->global_obj);
    one_obj_type_op_s *type_op = NULL;
    int ret;

    if (obj_attr == NULL) {
        core_log("[test_data] find obj_id[%u] failed\n", obj_id);
        return EC_OBJ_NOT_FOUND;
    }

    if (OBJ_UNION_TYPE(t, st) != OBJ_UNION_TYPE(obj_attr->obj_type, obj_attr->obj_subtype)) {
        core_log("[test_data] obj_id[%u] type[%u,%u] not match real type[%u,%u]\n",
            obj_id, t, st, obj_attr->obj_type, obj_attr->obj_subtype);
        return EC_OBJ_TYPE_INVALID;
    }

    ret = test_data_get_one_obj_type_op(t, st, &type_op);
    if (ret != EC_OK) {
        return ret;
    }

    return type_op->op[info->op](obj_attr, info->ret_val);
}

static int test_data_two_obj_op_proc(obj_op_info_s *info)
{
    u8 t1 = info->two_obj.obj1_type;
    u8 st1 = info->two_obj.obj1_subtype;
    u32 obj1_id = info->two_obj.obj1_id;
    u8 t2 = info->two_obj.obj2_type;
    u8 st2 = info->two_obj.obj2_subtype;
    u32 obj2_id = info->two_obj.obj2_id;
    obj_base_attr_s *obj1_attr = test_data_find_obj_by_id(obj1_id, info->global_obj);
    obj_base_attr_s *obj2_attr = test_data_find_obj_by_id(obj2_id, info->global_obj);
    two_obj_type_op_s *type_op = NULL;
    int ret;

    if (obj1_attr == NULL) {
        core_log("[test_data] find obj_id[%u] failed\n", obj1_id);
        return EC_OBJ_NOT_FOUND;
    }

    if (obj2_attr == NULL) {
        core_log("[test_data] find obj_id[%u] failed\n", obj2_id);
        return EC_OBJ_NOT_FOUND;
    }

    if (OBJ_UNION_2_TYPE(t1, st1, t2, st2) != 
        OBJ_UNION_2_TYPE(obj1_attr->obj_type, obj1_attr->obj_subtype,
                         obj2_attr->obj_type, obj2_attr->obj_subtype)) {
        core_log("[test_data] obj_id[%u|%u] type[%u,%u|%u,%u] not match real type[%u,%u|%u,%u]\n",
            obj1_id, obj2_id, t1, st1, t2, st2, obj1_attr->obj_type, obj1_attr->obj_subtype,
            obj2_attr->obj_type, obj2_attr->obj_subtype);
        return EC_OBJ_TYPE_INVALID;
    }

    ret = test_data_get_two_obj_type_op(t1, st1, t2, st2, &type_op);
    if (ret != EC_OK) {
        return ret;
    }

    return type_op->op[info->op - ONE_OBJ_OP_MAX](obj1_attr, obj2_attr, info->ret_val, info->ret_val_len);
}

int test_data_obj_op_proc(obj_op_info_s *info)
{
    if (info->op < ONE_OBJ_OP_MAX) {
        return test_data_one_obj_op_proc(info);
    } else if (info->op < TWO_OBJ_OP_MAX) {
        return test_data_two_obj_op_proc(info);
    } else {
        core_log("[test_data] type_op[%u] unsupport\n", info->op);
        return EC_UNSUPPORT_OP;
    }
}

static int test_data_obj_name_check(const char *obj_name, u32 parent_id, global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr = NULL;
    list_for_each_entry(obj_attr, &global_obj->obj_attr.node, node) {
        if (obj_attr->parent_id == parent_id && !libstr_strcmp(obj_name, obj_attr->obj_name)) {
            core_log("[test_data] obj_name[%s] exist under parent_id[%u]\n", obj_name, parent_id);
            return EC_OBJ_NAME_INVALID;
        }
    }
    return EC_OK;
}

static int test_data_get_obj_size(u8 obj_type, u8 obj_subtype, u8 *obj_size, bool is_root_mounted)
{
    static struct {
        u16 obj_ut;
        u8 obj_size;
    } obj_size_map[] = {
        { OBJ_UNION_TYPE(OBJ_TYPE_OBJECT, NO_OBJ_SUBTYPE),   sizeof(object_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_NUMBER, NUM_TYPE_INT),     sizeof(int_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT),   sizeof(float_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_NUMBER, NUM_TYPE_BOOL),    sizeof(bool_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_BOOL,   NO_OBJ_SUBTYPE),   sizeof(bool_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_NUMBER, NUM_TYPE_COMPLEX), sizeof(complex_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE),   sizeof(string_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_LIST,   NO_OBJ_SUBTYPE),   sizeof(list_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_TUPLE,  NO_OBJ_SUBTYPE),   sizeof(tuple_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_SET,    NO_OBJ_SUBTYPE),   sizeof(set_obj_s) },
        { OBJ_UNION_TYPE(OBJ_TYPE_DICT,   NO_OBJ_SUBTYPE),   sizeof(dict_obj_s) },
    };
    u16 obj_ut = OBJ_UNION_TYPE(obj_type, obj_subtype);
    int i;

    for (i = 0; i < ARRAY_SIZE(obj_size_map); i++) {
        if (obj_ut == obj_size_map[i].obj_ut) {
            log_printf(LOG_DEBUG, "[test_data] get_obj_size obj_type[%d] obj_subtype[%d] obj_size[%d]\n",
                obj_type, obj_subtype, obj_size_map[i].obj_size);
            *obj_size = obj_size_map[i].obj_size;
            return EC_OK;
        }
    }
    /* root_obj is created only once during global_obj initialization */
    if (CAN_ROOT_MOUNTED(obj_type, is_root_mounted)) {
        *obj_size = sizeof(root_obj_s);
        return EC_OK;
    }
    core_log("[test_data] obj_type[%u] not supported\n", obj_type);
    return EC_OBJ_TYPE_INVALID;
}

int test_data_obj_new(u8 obj_type, u8 obj_subtype, const char *obj_name, u32 parent_id,
                      global_obj_s *global_obj)
{
    obj_base_attr_s *parent_attr = NULL;
    obj_base_attr_s *obj_attr = NULL;
    void *obj = NULL;
    u8 obj_size = 0;
    int ret;

    parent_attr = test_data_find_obj_by_id(parent_id, global_obj);
    if (parent_attr == NULL) {
        core_log("[test_data] find parent_obj failed, parent_id[%u]\n", parent_id);
        return EC_OBJ_NOT_FOUND;
    }

    ret = test_data_obj_name_check(obj_name, parent_id, global_obj);
    if (ret != EC_OK) {
        core_log("[test_data] obj_name is invalid\n");
        return ret;
    }

    ret = test_data_get_obj_size(obj_type, obj_subtype, &obj_size, global_obj->is_root_mounted);
    if (ret != EC_OK) {
        core_log("[test_data] get obj_size failed, obj_type[%u] obj_subtype[%u]\n",
            obj_type, obj_subtype);
        return ret;
    }
    
    obj = mm_malloc(obj_size);
    if (obj == NULL) {
        core_log("[test_data] alloc obj failed, obj_size[%u]\n", obj_size);
        return EC_ALLOC_FAILED;
    }
    (void)mm_memset_s(obj, obj_size, 0, obj_size);

    /* promise obj_base_attr_s is the first member of obj */
    obj_attr = (obj_base_attr_s *)obj;
    obj_attr->obj_type = obj_type;
    obj_attr->obj_subtype = obj_subtype;
    obj_attr->free_flag = false;
    obj_attr->layer = parent_attr->layer + 1;
    obj_attr->obj_id = global_obj->obj_id_cnt;
    obj_attr->parent_id = parent_attr->obj_id;
    obj_attr->child_num = 0;
    obj_attr->obj_name = (char *)obj_name;
    INIT_LIST_HEAD(&obj_attr->node);
    list_add_tail(&obj_attr->node, &parent_attr->node);

    parent_attr->child_num++;
    global_obj->obj_id_cnt++;
    if (CAN_ROOT_MOUNTED(obj_type, global_obj->is_root_mounted)) {
        global_obj->is_root_mounted = true;
    }
    return ret;
}

int test_data_obj_del(u32 obj_id, global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr = test_data_find_obj_by_id(obj_id, global_obj);
    if (obj_attr == NULL) {
        core_log("[test_data] find obj_id[%u] failed\n", obj_id);
        return EC_OBJ_NOT_FOUND;
    }

    list_del(&obj_attr->node);
    mm_free(obj_attr);
    return EC_OK;
}

int test_data_obj_get_id_by_name(const char *obj_name, u32 parent_id, global_obj_s *global_obj,
                                 u32 *obj_id)
{
    obj_base_attr_s *obj_attr = test_data_find_obj_by_name(obj_name, parent_id, global_obj);
    if (obj_attr == NULL) {
        core_log("[test_data] find obj_name[%s] failed\n", obj_name);
        return EC_OBJ_NOT_FOUND;
    }
    *obj_id = obj_attr->obj_id;
    return EC_OK;
}
int test_data_init(global_obj_s **global_obj)
{
    static char global_obj_name[] = GLOBAL_OBJ_NAME;
    static char root_obj_name[] = ROOT_OBJ_NAME;
    obj_base_attr_s *global_obj_attr = NULL;
    int ret;

    BUILD_CHECK_OBJ_ATTR_SIZE();
    BUILD_CHECK_OBJ_SIZE();
    BUILD_CHECK_OBJ_OP_INFO_SIZE();

    if (*global_obj != NULL) {
        core_log("[test_data] global_obj shall be NULL before alloc\n");
        return EC_PARAM_INVALID;
    }

    *global_obj = mm_malloc(sizeof(global_obj_s));
    if (*global_obj == NULL) {
        core_log("[test_data] alloc global_obj failed\n");
        return EC_ALLOC_FAILED;
    }
    (*global_obj)->obj_id_cnt = 1;
    (*global_obj)->is_root_mounted = false;
    global_obj_attr = &(*global_obj)->obj_attr;
    global_obj_attr->obj_type = OBJ_TYPE_GLOBAL;
    global_obj_attr->free_flag = false;
    global_obj_attr->layer = GLOBAL_OBJ_LAYER;
    global_obj_attr->obj_id = GLOBAL_OBJ_ID;
    global_obj_attr->parent_id = GLOBAL_OBJ_ID;
    global_obj_attr->child_num = 0;
    global_obj_attr->obj_name = global_obj_name;
    INIT_LIST_HEAD(&global_obj_attr->node);

    /* new root obj */
    ret = test_data_obj_new(OBJ_TYPE_ROOT, NO_OBJ_SUBTYPE, root_obj_name,
                            GLOBAL_OBJ_ID, *global_obj);
    if (ret != EC_OK) {
        core_log("[test_data] new obj failed, ret[%d]\n", ret);
        return ret;
    }
    return EC_OK;
}

int test_data_free(global_obj_s **global_obj)
{
    return EC_OK;
}

void test_data_print_obj_list(global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr = NULL;
    core_log("================================ START ================================\n");
    list_for_each_entry(obj_attr, &global_obj->obj_attr.node, node) {
        core_log("[test_data] obj_id[%u]\t obj_name[%s]\t obj_type[%u]\t obj_subtype[%u]\t "
                    "layer[%u]\t parent_id[%u]\t child_num[%u]\n",
                    obj_attr->obj_id, obj_attr->obj_name, obj_attr->obj_type, obj_attr->obj_subtype,
                    obj_attr->layer, obj_attr->parent_id, obj_attr->child_num);
    }
    core_log("================================= END =================================\n");
}