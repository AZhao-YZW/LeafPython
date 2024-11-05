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
#include "libstr.h"
#include "error_code.h"
#include "log.h"

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64)

#include <string.h>
static int leafpy_strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

static int leafpy_strlen(const char *str)
{
    return strlen(str);
}

static int leafpy_strcat_s(char *dst, u32 bytes, const char *src)
{
    return strcat_s(dst, bytes, src);
}

static int leafpy_strcpy_s(char *dst, u32 bytes, const char *src)
{
    return strcpy_s(dst, bytes, src);
}

#else
#error "unsupported platform"
#endif

int libstr_strcmp(const char *str1, const char *str2)
{
    return leafpy_strcmp(str1, str2);
}

int libstr_strlen(const char *str)
{
    return leafpy_strlen(str);
}

int libstr_strcat_s(char *dst, u32 bytes, const char *src)
{
    int ret = leafpy_strcat_s(dst, bytes, src);
    if (ret != 0) {
        core_log("leafpy_strcat_s failed, ret[%d]\n", ret);
        return EC_STRING_OPTION_FAILED;
    }
}

int libstr_strcpy_s(char *dst, u32 bytes, const char *src)
{
    int ret = leafpy_strcpy_s(dst, bytes, src);
    if (ret != 0) {
        core_log("leafpy_strcpy_s failed, ret[%d]\n", ret);
        return EC_STRING_OPTION_FAILED;
    }
}