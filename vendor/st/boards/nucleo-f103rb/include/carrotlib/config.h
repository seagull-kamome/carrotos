/*
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
 */

/** @file
 *  @brief Configuration file of carrotlib for st.nucleo-f103rb.
 */
#pragma once

//#define CARROT_CONFIG_SMP             (0)
#define CARROT_CONFIG_BITOPS_IMPL       CARROT_BITOPS_IMPL_BUILTIN
#define CARROT_CONFIG_ATOMIC_IMPL       CARROT_ATOMIC_IMPL_NONE
#define CARROT_CONFIG_SPINLOCK_IMPL     CARROT_SPINLOCK_IMPL_NONE
#define CARROT_CONFIG_IRQ_IMPL          CARROT_IRQ_IMPL_ARCH
#define CARROT_CONFIG_TIMER_IMPL        CARROT_TIMER_IMPL_ARCH
#undef CARROT_CONFIG_LARGE_TIME_T
#define CARROT_CONFIG_CPU_IMPL          CARROT_CPU_IMPL_ARCH



// vim: ts=8 sw=2 cindent expandtab :
