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

static void test_data_obj_base_attr_init(u32 obj_id, char *obj_name, obj_base_attr_s *parent_attr,
                                         obj_base_attr_s *obj_attr)
{
    obj_attr->obj_type = OBJ_TYPE_GLOBAL;
    obj_attr->free_flag = false;
    obj_attr->layer = parent_attr->layer + 1;
    obj_attr->obj_id = obj_id;
    obj_attr->parent_id = parent_attr->parent_id;
    obj_attr->child_num = 0;
    obj_attr->obj_name = obj_name;
    INIT_LIST_HEAD(&obj_attr->node);
    list_add_tail(&obj_attr->node, &parent_attr->node);
}

static obj_base_attr_s *test_data_find_obj(u32 obj_id, global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr;

    if (obj_id == GLOBAL_OBJ_ID) {
        return &global_obj->obj_attr;
    }

    list_for_each_entry(obj_attr, &global_obj->obj_attr.node, node) {
        log_printf(LOG_DEBUG, "[test_data] find obj_attr, obj_id[%u]\n", obj_attr->obj_id);
        if (obj_attr->obj_id == obj_id) {
            return obj_attr;
        }
    }
    return NULL;
}

static int test_data_object_obj_new(char *obj_name, obj_base_attr_s *parent_attr, global_obj_s *global_obj)
{
    object_obj_s *object_obj = mm_malloc(sizeof(*object_obj));
    if (object_obj == NULL) {
        core_printf("[test_data] alloc object_obj failed, parent_id[%u]\n", parent_attr->parent_id);
        return EC_ALLOC_FAILED;
    }
    test_data_obj_base_attr_init(global_obj->obj_id_cnt, obj_name, parent_attr, &object_obj->obj_attr);
    return EC_OK;
}

int test_data_obj_new(u8 obj_type, char *obj_name, u32 parent_id, global_obj_s *global_obj)
{
    obj_base_attr_s *parent_attr = NULL;
    int ret;

    parent_attr = test_data_find_obj(parent_id, global_obj);
    if (parent_attr == NULL) {
        core_printf("[test_data] find parent_obj failed, parent_id[%u]\n", parent_id);
        return EC_OBJ_ID_INVALID;
    }

    switch (obj_type) {
        case OBJ_TYPE_OBJECT:
            ret = test_data_object_obj_new(obj_name, parent_attr, global_obj);
            break;
        default:
            core_printf("[test_data] obj_type[%d] not supported\n", obj_type);
            return EC_OBJ_TYPE_INVALID;
    }
    if (ret == EC_OK) {
        global_obj->obj_id_cnt++;
    }
    return ret;
}

int test_data_obj_del(u32 obj_id, global_obj_s *global_obj)
{
    obj_base_attr_s *obj_attr = test_data_find_obj(obj_id, global_obj);
    if (obj_attr == NULL) {
        core_printf("[test_data] find obj_id[%u] failed\n", obj_id);
        return EC_OBJ_ID_INVALID;
    }

    list_del(&obj_attr->node);
    mm_free(obj_attr);
    return EC_OK;
}

int test_data_init(global_obj_s **global_obj)
{
    static char global_obj_name[] = "global_obj";
    static char root_obj_name[] = "root_obj";
    obj_base_attr_s *global_obj_attr = NULL;
    int ret;

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
    ret = test_data_obj_new(OBJ_TYPE_OBJECT, root_obj_name, GLOBAL_OBJ_ID, *global_obj);
    if (ret != EC_OK) {
        core_printf("[test_data] new obj failed, ret[%d]\n", ret);
        return ret;
    }
    return EC_OK;
}