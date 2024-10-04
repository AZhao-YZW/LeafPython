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
#include "leafpy_api.h"
#include <stdio.h>

int test_c_func(int arg1, char *arg2, float arg3)
{
    printf("test_c_func: %d, %s, %.2f\n", arg1, arg2, arg3);
    return 0;
}

int module_register(void)
{
    int ret;
    ret = LEAFPY_REGISTER_CFUNC("test_c",
                                "test_c_func",
                                CFUNC_TYPE(int, 0),
                                3,
                                CFUNC_TYPE(int, 0),
                                CFUNC_TYPE(char, 1),
                                CFUNC_TYPE(float, 0));
    if (ret != 0) {
        printf("LEAFPY_REGISTER_CFUNC failed, ret[%d]\n", ret);
        return -1;
    }
    return 0;
}