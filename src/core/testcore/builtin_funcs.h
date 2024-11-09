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
#ifndef _BUILTIN_FUNCS_H_
#define _BUILTIN_FUNCS_H_
#ifdef __cplusplus
extern "C" {
#endif

enum builtin_func_e {
    BUILTIN_FUNC_abs,
    BUILTIN_FUNC_aiter,
    BUILTIN_FUNC_all,
    BUILTIN_FUNC_anext,
    BUILTIN_FUNC_any,
    BUILTIN_FUNC_ascii,
    BUILTIN_FUNC_bin,
    BUILTIN_FUNC_bool,
    BUILTIN_FUNC_breakpoint,
    BUILTIN_FUNC_bytearray,
    BUILTIN_FUNC_bytes,
    BUILTIN_FUNC_callable,
    BUILTIN_FUNC_chr,
    BUILTIN_FUNC_classmethod,
    BUILTIN_FUNC_compile,
    BUILTIN_FUNC_complex,
    BUILTIN_FUNC_delattr,
    BUILTIN_FUNC_dict,
    BUILTIN_FUNC_dir,
    BUILTIN_FUNC_divmod,
    BUILTIN_FUNC_enumerate,
    BUILTIN_FUNC_eval,
    BUILTIN_FUNC_exec,
    BUILTIN_FUNC_filter,
    BUILTIN_FUNC_float,
    BUILTIN_FUNC_format,
    BUILTIN_FUNC_frozenset,
    BUILTIN_FUNC_getattr,
    BUILTIN_FUNC_globals,
    BUILTIN_FUNC_hasattr,
    BUILTIN_FUNC_hash,
    BUILTIN_FUNC_help,
    BUILTIN_FUNC_hex,
    BUILTIN_FUNC_id,
    BUILTIN_FUNC_input,
    BUILTIN_FUNC_int,
    BUILTIN_FUNC_isinstance,
    BUILTIN_FUNC_issubclass,
    BUILTIN_FUNC_iter,
    BUILTIN_FUNC_len,
    BUILTIN_FUNC_list,
    BUILTIN_FUNC_locals,
    BUILTIN_FUNC_map,
    BUILTIN_FUNC_max,
    BUILTIN_FUNC_memoryview,
    BUILTIN_FUNC_min,
    BUILTIN_FUNC_next,
    BUILTIN_FUNC_object,
    BUILTIN_FUNC_oct,
    BUILTIN_FUNC_open,
    BUILTIN_FUNC_ord,
    BUILTIN_FUNC_pow,
    BUILTIN_FUNC_print,
    BUILTIN_FUNC_property,
    BUILTIN_FUNC_range,
    BUILTIN_FUNC_repr,
    BUILTIN_FUNC_reversed,
    BUILTIN_FUNC_round,
    BUILTIN_FUNC_set,
    BUILTIN_FUNC_setattr,
    BUILTIN_FUNC_slice,
    BUILTIN_FUNC_sorted,
    BUILTIN_FUNC_staticmethod,
    BUILTIN_FUNC_str,
    BUILTIN_FUNC_sum,
    BUILTIN_FUNC_super,
    BUILTIN_FUNC_tuple,
    BUILTIN_FUNC_type,
    BUILTIN_FUNC_vars,
    BUILTIN_FUNC_zip,
    BUILTIN_FUNC_MAX
};

#ifdef __cplusplus
}
#endif
#endif