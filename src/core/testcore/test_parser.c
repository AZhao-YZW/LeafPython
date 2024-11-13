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
#include "list.h"
#include "mm.h"
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

static int test_parser_parse_line(char *line, u32 line_len, parser_line_s *parser_line)
{
    if (line != NULL && line_len != 0) {
        return PARSE_LINE_NEXT_LINE;
    } else {
        return PARSE_LINE_FRAME_END;
    }
}

static test_frame_s *test_parser_combine_frame(parser_line_s *parser_line)
{
    u32 bc_num = 0;
    test_frame_s *frame = NULL;
    parser_line_bc_s *line_bc;
    parser_line_bc_s *next;

    if (list_empty(&parser_line->line_bc_head)) {
        return NULL;
    }

    frame = test_frame_create(parser_line->bc_num);
    if (frame == NULL) {
        core_log("[test_parser] create frame failed\n");
        return NULL;
    }

    list_for_each_entry_safe(line_bc, next, &parser_line->line_bc_head, node) {
        frame->bc_list[bc_num] = line_bc->bc;
        list_del(&line_bc->node);
        mm_free(line_bc);
        bc_num++;
    }
    return frame;
}

int test_parser_parse_code(const char *code, u32 code_len, u8 frame_q_id)
{
    char *code_end = (char *)code + code_len;
    char *line = (char *)code;
    char *line_end;
    parser_line_s parser_line = {0};
    test_frame_s *frame = NULL;
    int status, ret;

    if (code == NULL) {
        core_log("[test_parser] code is NULL\n");
        return EC_PARAM_INVALID;
    }

    INIT_LIST_HEAD(&parser_line.line_bc_head);
    while (line < code_end) {
        line_end = libstr_strchr(line, '\n');
        if (line_end == NULL) {
            line_end = code_end;
        }
        status = test_parser_parse_line(line, line_end - line, &parser_line);
        if (status == PARSE_LINE_NEXT_LINE) {
            line = line_end;
            continue;
        } else if(status == PARSE_LINE_FRAME_END) {
            frame = test_parser_combine_frame(&parser_line);
            if (frame == NULL) {
                core_log("[test_parser] combine frame failed\n");
                return EC_FRAME_COMBIND_ERROR;
            }
            ret = test_frame_enqueue(frame_q_id, frame);
            if (ret != EC_OK) {
                core_log("[test_parser] enqueue frame failed\n");
                return ret;
            }
        } else {
            core_log("[test_parser] parse line failed\n");
        }
    }
    return EC_OK;
}

int test_parser_parse_repl(const char *line, u32 line_len, u8 frame_q_id)
{
    return EC_OK;
}