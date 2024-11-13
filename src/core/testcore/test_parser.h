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
#ifndef _TEST_PARSER_H_
#define _TEST_PARSER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include "list.h"
#include "test_frame.h"

enum parser_syntex_error_code {
    SYNTEX_OK,
    SYNTEX_ERROR,
    SYNTEX_ERROR_MAX
};

enum parse_line_status {
    PARSE_LINE_NEXT_LINE,
    PARSE_LINE_ERROR,
    PARSE_LINE_FRAME_END,
    PARSE_LINE_MAX_STATUS
};

typedef struct {
    struct list_head node;
    test_bc_s bc;
} parser_line_bc_s;

typedef struct {
    struct list_head line_bc_head;
    u32 bc_num;
} parser_line_s;

/**
 * The function of parser, frame, frame queue, vm and core are as follows:
 * 1. Parser: Parses code to bytecode *line by line*. When these lines form a block that 
 *    needs to be interpreted completely, they are combined into a frame, which the parser 
 *    then enqueues into the frame queue.
 * 2. Frame: A list of bytecodes that need to be interpreted together.
 * 3. Frame Queue: The communication channel between the parser and the VM. The parser 
 *    enqueues a frame, and the VM dequeues it to interpret the bytecodes.
 * 4. VM: Interprets the bytecodes in the frames.
 * 5. Core: Implements the low-level operations for each specific bytecode instruction.
 */
int test_parser_parse_code(const char *code, u32 code_len, u8 frame_q_id);
int test_parser_parse_repl(const char *line, u32 line_len, u8 frame_q_id);

#ifdef __cplusplus
}
#endif
#endif