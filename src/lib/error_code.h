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
#ifndef _ERROR_H_
#define _ERROR_H_
#ifdef __cplusplus
extern "C" {
#endif

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

enum error_code_e {
    EC_OK                       = 0,
    EC_PARAM_INVALID            = 1,
    EC_ALLOC_FAILED             = 2,
    EC_UNSUPPORT_OP             = 3,
    EC_TIMEOUT                  = 4,
    EC_UNKNOWN                  = 5,
    EC_CORE_ID_INVALID          = 6,
    EC_CORE_TYPE_INVALID        = 7,
    EC_OBJ_TYPE_INVALID         = 8,
    EC_OBJ_ID_INVALID           = 9,
    EC_OBJ_NOT_DELETABLE        = 10,
    EC_OBJ_NOT_FOUND            = 11,
    EC_OBJ_NAME_INVALID         = 12,
    EC_OBJ_NAME_LEN_EXCEED      = 13,
    EC_STRING_OPTION_FAILED     = 14,
    EC_MAY_SUPPORT_LATER        = 15,
    EC_MAX                      = 0xFFFF
};

#ifdef __cplusplus
}
#endif
#endif