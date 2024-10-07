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
#include "register.h"
#include <stdarg.h>

typedef struct {
    const char *module;
    const char *pyfunc;
    const void *cfunc;
    leafpy_cfunc_type ret_type;
    unsigned int arg_num;
    leafpy_cfunc_type *arg_types;
} leafpy_register_info_s;

static int register_cfunc(leafpy_register_info_s *info)
{
    return 0;
}

/*****************************************************************************
 *                            Register C functions
 *****************************************************************************/
int leafpy_register_cfunc(const char *module, const char *pyfunc, const void *cfunc,
                          leafpy_cfunc_type ret_type, unsigned int arg_num, ...)
{
    leafpy_register_info_s info = {0};
    leafpy_cfunc_type arg_types[arg_num];
    int i, ret;
    va_list ap;
    va_start(ap, arg_num);
    info.module = module;
    info.pyfunc = pyfunc;
    info.cfunc = cfunc;
    info.ret_type = ret_type;
    info.arg_num = arg_num;
    info.arg_types = arg_types;
    for (i = 0; i < arg_num; i++) {
        // ... auto convert type to int
        info.arg_types[i] = (leafpy_cfunc_type)va_arg(ap, int);
    }
    va_end(ap);
    ret = register_cfunc(&info);
    return ret;
}