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
#include "error.h"
#include "list.h"
#include "mm.h"
#include "test_frame.h"

struct list_head g_vm_list;
bool is_vm_first_init = true;

int test_vm_init(u8 core_id)
{
    test_vm_s *vm = mm_malloc(sizeof(test_vm_s));
    if (vm == NULL) {
        return EC_ALLOC_FAILED;
    }
    vm->core_id = core_id;

    if (is_vm_first_init) {
        INIT_LIST_HEAD(&g_vm_list);
        is_vm_first_init = false;
    }
    list_add_tail(&vm->node, &g_vm_list);
    return EC_OK;
}

void test_vm_free(u8 core_id)
{
    test_vm_s *vm = NULL;
    list_for_each_entry(vm, &g_vm_list, node) {
        if (vm->core_id == core_id) {
            list_del(&vm->node);
            mm_free(vm);
            return;
        }
    }
}