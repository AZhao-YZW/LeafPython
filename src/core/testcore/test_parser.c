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
#include "test_parser.h"
#include "error_code.h"
#include "log.h"
#include "libstr.h"
#include "test_frame.h"

char *g_py_keywords[] = {
    "False", "None", "True",
    "and", "as", "assert", "break", "class", "continue",
    "def", "del", "elif", "else", "except", "finally",
    "for", "from", "global", "if",  "import", "in",
    "is", "lambda", "nonlocal", "not", "or", "pass",
    "raise", "return", "try", "while", "with", "yield",
    "async", "await"
};

char *g_py_operators[] = {
    "+", "-", "*", "**", "/", "//", "%", "@",
    "<<", ">>", "&", "|", "^", "~", ":=",
    "<", ">", "<=", ">=", "==", "!="
};

char *g_py_delimiters[] = {
    "(", ")", "[", "]", "{", "}",
    ",", ":", "!", ".", ";", "@", "=",
    "->", "+=", "-=", "*=", "/=", "//=", "%=",
    "@=", "&=", "|=", "^=", ">>=", "<<=", "**="
};

static u8 test_parser_get_bc_num(const char *code, u32 code_len)
{
    return 0;
}

static int test_parser_parse_line(char *line, u32 line_len, u32 *bc_num)
{
    *bc_num += 1;
    return EC_OK;
}

static int test_parser_parse_frame(char *frame_start, char *frame_end, u32 *bc_num)
{
    char *line = frame_start;
    char *line_end;
    u32 line_len;
    int ret;

    while (line < frame_end) {
        line_end = libstr_strchr(line, '\n');
        if (line_end == NULL) {
            line_end = frame_end;
        }
        line_len = line_end - line;
        ret = test_parser_parse_line(line, line_len, bc_num);
        if (ret != EC_OK) {
            core_log("[test_parser] parse line failed, line: %s\n", line);
            return ret;
        }
        line = line_end;
    }
    return EC_OK;
}

static char *test_parser_get_frame_end(char *frame_start, u32 remain_len)
{
    return NULL;
}

int test_parser_parse_code(const char *code, u32 code_len)
{
    const char *code_end = code + code_len;
    char *frame_start = (char *)code;
    char *frame_end;
    u32 bc_num = 0;
    test_frame_s *frame = NULL;
    int ret;

    if (code == NULL) {
        core_log("[test_parser] code is NULL\n");
        return EC_PARAM_INVALID;
    }

    while (frame_start < code_end) {
        frame_end = test_parser_get_frame_end(frame_start, code - frame_start);
        if (frame_end == NULL) {
            frame_end =(char *) code_end;
        }
        ret = test_parser_parse_frame(frame_start, frame_end, &bc_num);
        if (ret != EC_OK) {
            core_log("[test_parser] parse frame failed\n");
            return ret;
        }
        frame = test_frame_create(bc_num);
        if (frame == NULL) {
            core_log("[test_parser] create frame failed\n");
            return EC_FRAME_CREATE_FAILED;
        }
        ret = test_frame_enqueue(TEST_FRAME_QUEUE_0, frame);
        if (ret != EC_OK) {
            core_log("[test_parser] enqueue frame failed\n");
            return ret;
        }
        frame_start = frame_end;
    }
    return EC_OK;
}