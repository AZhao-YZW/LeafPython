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
        core_printf("[test_data] global_obj is NULL\n");
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
        core_printf("[test_data] global_obj is NULL\n");
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

DEFINE_SET_OBJ_VAL_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_INT, offsetof(int_obj_s, val), s64)
DEFINE_SET_OBJ_VAL_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT, offsetof(float_obj_s, val), f64)
DEFINE_SET_OBJ_VAL_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_BOOL, offsetof(bool_obj_s, val), u8)
DEFINE_SET_OBJ_VAL_FUNC(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE, offsetof(string_obj_s, val), char *)

DEFINE_GET_OBJ_VAL_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_INT, offsetof(int_obj_s, val), s64)
DEFINE_GET_OBJ_VAL_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT, offsetof(float_obj_s, val), f64)
DEFINE_GET_OBJ_VAL_FUNC(OBJ_TYPE_NUMBER, NUM_TYPE_BOOL, offsetof(bool_obj_s, val), u8)
DEFINE_GET_OBJ_VAL_FUNC(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE, offsetof(string_obj_s, val), char *)

static test_data_type_op_s type_op_map[] = {
    TEST_DATA_TYPE_OP_ELEMENT(OBJ_TYPE_NUMBER, NUM_TYPE_INT),
    TEST_DATA_TYPE_OP_ELEMENT(OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT),
    TEST_DATA_TYPE_OP_ELEMENT(OBJ_TYPE_NUMBER, NUM_TYPE_BOOL),
    TEST_DATA_TYPE_OP_ELEMENT(OBJ_TYPE_STRING, NO_OBJ_SUBTYPE),
};

static int test_data_get_type_op(u8 obj_type, u8 obj_subtype, test_data_type_op_s **type_op)
{
    u16 obj_union_type = OBJ_UNION_TYPE(obj_type, obj_subtype);
    int i;
    for (i = 0; i < ARRAY_SIZE(type_op_map); i++) {
        if (obj_union_type == type_op_map[i].obj_union_type) {
            *type_op = &type_op_map[i];
            return EC_OK;
        }
    }
    core_printf("[test_data] unsupport type op: [%d, %d]\n", obj_type, obj_subtype);
    return EC_OBJ_TYPE_INVALID;
}

static int test_data_obj_name_check(const char *obj_name, u32 parent_id, global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr = NULL;
    list_for_each_entry(obj_attr, &global_obj->obj_attr.node, node) {
        if (obj_attr->parent_id == parent_id && !libstr_strcmp(obj_name, obj_attr->obj_name)) {
            core_printf("[test_data] obj_name[%s] exist under parent_id[%u]\n", obj_name, parent_id);
            return EC_OBJ_NAME_INVALID;
        }
    }
    return EC_OK;
}

static int test_data_get_obj_size(u8 obj_type, u8 obj_subtype, u8 *obj_size, bool is_root_mounted)
{
    static struct {
        u8 obj_type;
        u8 obj_subtype;
        u8 obj_size;
    } obj_size_map[] = {
        { OBJ_TYPE_OBJECT, NO_OBJ_SUBTYPE,   sizeof(object_obj_s) },
        { OBJ_TYPE_NUMBER, NUM_TYPE_INT,     sizeof(int_obj_s) },
        { OBJ_TYPE_NUMBER, NUM_TYPE_FLOAT,   sizeof(float_obj_s) },
        { OBJ_TYPE_NUMBER, NUM_TYPE_BOOL,    sizeof(bool_obj_s) },
        { OBJ_TYPE_BOOL,   NO_OBJ_SUBTYPE,   sizeof(bool_obj_s) },
        { OBJ_TYPE_NUMBER, NUM_TYPE_COMPLEX, sizeof(complex_obj_s) },
        { OBJ_TYPE_STRING, NO_OBJ_SUBTYPE,   sizeof(string_obj_s) },
        { OBJ_TYPE_LIST,   NO_OBJ_SUBTYPE,   sizeof(list_obj_s) },
        { OBJ_TYPE_TUPLE,  NO_OBJ_SUBTYPE,   sizeof(tuple_obj_s) },
        { OBJ_TYPE_SET,    NO_OBJ_SUBTYPE,   sizeof(set_obj_s) },
        { OBJ_TYPE_DICT,   NO_OBJ_SUBTYPE,   sizeof(dict_obj_s) },
    };
    int i;

    for (i = 0; i < ARRAY_SIZE(obj_size_map); i++) {
        if (obj_type == obj_size_map[i].obj_type && obj_subtype == obj_size_map[i].obj_subtype) {
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
    core_printf("[test_data] obj_type[%d] not supported\n", obj_type);
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
        core_printf("[test_data] find parent_obj failed, parent_id[%u]\n", parent_id);
        return EC_OBJ_NOT_FOUND;
    }

    ret = test_data_obj_name_check(obj_name, parent_id, global_obj);
    if (ret != EC_OK) {
        core_printf("[test_data] obj_name is invalid\n");
        return ret;
    }

    ret = test_data_get_obj_size(obj_type, obj_subtype, &obj_size, global_obj->is_root_mounted);
    if (ret != EC_OK) {
        core_printf("[test_data] get obj_size failed, obj_type[%u] obj_subtype[%u]\n",
            obj_type, obj_subtype);
        return ret;
    }
    
    obj = mm_malloc(obj_size);
    if (obj == NULL) {
        core_printf("[test_data] alloc obj failed, obj_size[%u]\n", obj_size);
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
        core_printf("[test_data] find obj_id[%u] failed\n", obj_id);
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
        core_printf("[test_data] find obj_name[%s] failed\n", obj_name);
        return EC_OBJ_NOT_FOUND;
    }
    *obj_id = obj_attr->obj_id;
    return EC_OK;
}

int test_data_obj_op_proc(test_data_obj_op_info_s *info)
{
    u8 obj_id = info->obj_id;
    u8 obj_type = info->obj_type;
    u8 obj_subtype = info->obj_subtype;
    obj_base_attr_s *obj_attr = test_data_find_obj_by_id(obj_id, info->global_obj);
    test_data_type_op_s *type_op = NULL;
    int ret;

    if (obj_attr == NULL) {
        core_printf("[test_data] find obj_id[%u] failed\n", obj_id);
        return EC_OBJ_NOT_FOUND;
    }

    if (obj_type != obj_attr->obj_type || obj_subtype != obj_attr->obj_subtype) {
        core_printf("[test_data] obj_id[%u] type[%u, %u] not match real type[%u, %u]\n",
            obj_id, obj_type, obj_subtype, obj_attr->obj_type, obj_attr->obj_subtype);
        return EC_OBJ_TYPE_INVALID;
    }

    ret = test_data_get_type_op(obj_type, obj_subtype, &type_op);
    if (ret != EC_OK) {
        return ret;
    }

    switch (info->op) {
        case TYPE_OP_SET_OBJ_VAL:
            type_op->set_obj_val((void *)obj_attr, info->param.set_get_obj_val.val);
            break;
        case TYPE_OP_GET_OBJ_VAL:
            type_op->get_obj_val(info->param.set_get_obj_val.val, (void *)obj_attr);
            break;
        default:
            core_printf("[test_data] type_op[%u] unsupport\n", info->op);
            return EC_UNSUPPORT_OP;
    }
    return EC_OK;
}

static int test_data_obj_add_proc(obj_base_attr_s *obj1_attr, obj_base_attr_s *obj2_attr,
                                  void *obj_val, u32 val_len)
{
    u8 t1 = obj1_attr->obj_type;
    u8 st1 = obj1_attr->obj_subtype;
    u8 t2 = obj2_attr->obj_type;
    u8 st2 = obj2_attr->obj_subtype;

    switch (OBJ_UNION_2_TYPE(t1, st1, t2, st2)) {
        case OBJ_UNION_2_TYPE_SIMPLE(NUMBER, NUM_TYPE_INT, NUMBER, NUM_TYPE_INT):
            *(s64 *)obj_val = ((int_obj_s *)obj1_attr)->val + ((int_obj_s *)obj2_attr)->val;
            return EC_OK;
        case OBJ_UNION_2_TYPE_SIMPLE(NUMBER, NUM_TYPE_FLOAT, NUMBER, NUM_TYPE_FLOAT):
            *(f64 *)obj_val = ((float_obj_s *)obj1_attr)->val + ((float_obj_s *)obj2_attr)->val;
            return EC_OK;
        case OBJ_UNION_2_TYPE_SIMPLE(NUMBER, NUM_TYPE_BOOL, NUMBER, NUM_TYPE_BOOL):
            *(u8 *)obj_val = ((bool_obj_s *)obj1_attr)->val + ((bool_obj_s *)obj2_attr)->val;
            return EC_OK;
        case OBJ_UNION_2_TYPE_SIMPLE(STRING, NO_OBJ_SUBTYPE, STRING, NO_OBJ_SUBTYPE):
            char *str = *(char **)obj_val;
            u32 str_len = val_len;
            u32 str1_len = libstr_strlen(((string_obj_s *)obj1_attr)->val);
            u32 str2_len = libstr_strlen(((string_obj_s *)obj2_attr)->val);
            int ret;
            ret = libstr_strcpy_s(str, str_len, ((string_obj_s *)obj1_attr)->val);
            if (ret != EC_OK) {
                return ret;
            }
            ret = libstr_strcat_s(str, str_len, ((string_obj_s *)obj2_attr)->val);
            if (ret != EC_OK) {
                return ret;
            }
            return EC_OK;
        case OBJ_UNION_2_TYPE_SIMPLE(NUMBER, NUM_TYPE_INT, STRING, NO_OBJ_SUBTYPE):
        case OBJ_UNION_2_TYPE_SIMPLE(STRING, NO_OBJ_SUBTYPE, NUMBER, NUM_TYPE_INT):
            return EC_MAY_SUPPORT_LATER;
        default:
            core_printf("[test_data] unsupport type op: [%d, %d] add [%d, %d]\n", t1, st1, t2, st2);
            return EC_OBJ_TYPE_INVALID;
    }
}

int test_data_obj_add(u32 obj1_id, u32 obj2_id, u32 val_len, global_obj_s *global_obj, void *obj_val)
{
    obj_base_attr_s *obj1_attr = test_data_find_obj_by_id(obj1_id, global_obj);
    obj_base_attr_s *obj2_attr = test_data_find_obj_by_id(obj2_id, global_obj);
    int ret;

    if (obj1_attr == NULL || obj2_attr == NULL) {
        core_printf("[test_data] find obj failed, obj1_id[%u] obj2_id[%u]\n", obj1_id, obj2_id);
        return EC_OBJ_NOT_FOUND;
    }

    if (obj_val == NULL) {
        core_printf("[test_data] obj_val is NULL\n");
        return EC_PARAM_INVALID;
    }

    ret = test_data_obj_add_proc(obj1_attr, obj2_attr, obj_val, val_len);
    if (ret != EC_OK) {
        core_printf("[test_data] add obj failed, obj1_id[%u] obj2_id[%u]\n", obj1_id, obj2_id);
        return ret;
    }
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

    if (*global_obj != NULL) {
        core_printf("[test_data] global_obj shall be NULL before alloc\n");
        return EC_PARAM_INVALID;
    }

    *global_obj = mm_malloc(sizeof(global_obj_s));
    if (*global_obj == NULL) {
        core_printf("[test_data] alloc global_obj failed\n");
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
        core_printf("[test_data] new obj failed, ret[%d]\n", ret);
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
    core_printf("================================ START ================================\n");
    list_for_each_entry(obj_attr, &global_obj->obj_attr.node, node) {
        core_printf("[test_data] obj_id[%u]\t obj_name[%s]\t obj_type[%u]\t obj_subtype[%u]\t "
                    "layer[%u]\t parent_id[%u]\t child_num[%u]\n",
                    obj_attr->obj_id, obj_attr->obj_name, obj_attr->obj_type, obj_attr->obj_subtype,
                    obj_attr->layer, obj_attr->parent_id, obj_attr->child_num);
    }
    core_printf("================================= END =================================\n");
}