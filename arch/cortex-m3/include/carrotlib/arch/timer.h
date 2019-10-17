/**
 * @file
 * @brief Interface to HW timer for ARMCM3.
 * @section LICENSE
 *
 * MIT License
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
 *
 * @section Related configuration vars.
 *
 *   - CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ
 *   - CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC
 */
#pragma once
#include <stdbool.h>
#include <carrotlib/config.h>
#include <carrotlib/compiler.h>
#include <carrotlib/time.h>

#define CARROT_ARMCM3_SYSTICK_CLKSRC_SYS   (0)
#define CARROT_ARMCM3_SYSTICK_CLKSRC_EXT   (1)

void carrot_arch_clock_start_hw_timer(void);
bool carrot_arch_clock_get_hw_elapsed(struct carrot_timespec *ts);
void carrot_arch_clock_set_hw_alarm(unsigned int delay_usec);

void carrot_armcm3_systick_isr(void);

#define CARROT_ARCH_CLOCK_HW_HAS_ALARM          (1)


// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
