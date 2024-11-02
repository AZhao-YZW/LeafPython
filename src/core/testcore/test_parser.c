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

int test_parser_bc_gen_frame(const char *bytecode, u32 bytecode_len, test_frame_s *frame)
{
    return EC_OK;
}

u32 test_parser_get_frame_bc_num(const char *line, u32 line_len)
{
    return 1;
}

int test_parser_line_to_frame(const char *line, u32 line_len, test_frame_s *frame)
{
    return EC_OK;
}

u32 test_parser_get_line_len(const char *code, u32 code_len, u32 offset)
{
    return 10;
}