/**
 * @file
 * @brief Interface of real-time clock
 * @section LICENSE
 *
 * Copyright (c) 2019 Hattori, Hiroki
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once
#include <stdint.h>
#include <carrotlib/compiler.h>

CARROT_BEGIN_EXTERN_C // {

extern struct carrot_cstartup_data_section_layout {
  uint8_t *loadaddr;
  uint8_t *execaddr;
  uint8_t *exeaddr_tail;
} carrot_cstartup_data_sections[];


extern struct carrot_cstartup_bss_section_layout {
  uint8_t *bssaddr;
  uint8_t *bssaddr_tail;
} carrot_cstartup_bss_sections[];


void carrot_cstartup_relocate_sections(void);

CARROT_END_EXTERN_C // }

// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
