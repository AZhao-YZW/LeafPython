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
#include "log.h"
#include "type.h"
#include "leafpy_cfg.h"
#include <stdarg.h>
#if defined(__linux__) || defined(_WIN32) || defined(_WIN64)
#include <stdio.h>
#else
void printf(const char *fmt, va_list ap) {}
#endif

u8 g_log_level = LEAFPY_LOG_LEVEL;

void log_set_level(enum log_level_e level)
{
    g_log_level = level;
}

void log_printf(enum log_level_e level, const char *fmt, ...)
{
    char *log_level_str[] = { "debug", "info ", "warn ", "error", "fatal" };
    va_list ap;
    va_start(ap, fmt);
    if (level <= g_log_level) {
        printf("[%s] ", log_level_str[level]);
        printf(fmt, ap);
    }
    va_end(ap);
}

void core_printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf("[core] ");
    printf(fmt, ap);
    va_end(ap);
}