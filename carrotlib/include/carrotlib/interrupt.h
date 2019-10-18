/**
 * @file
 * @brief Enable/Disable interrupts.
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
 */
#pragma once


#define CARROT_IRQ_IMPL_NONE      (0)
#define CARROT_IRQ_IMPL_ARCH      (100)
#define CARROT_IRQ_IMPL_MCU       (101)
#define CARROT_IRQ_IMPL_BOARD     (102)
#define CARROT_IRQ_IMPL_CONFIG    (103)

#if !defined(CARROT_CONFIG_IRQ_IMPL)
#  error "CARROT_CONFIG_IRQ_IMPL not defined."

/* ************************************************************************ */
#elif (CARROT_CONFIG_IRQ_IMPL == CARROT_IRQ_IMPL_NONE)

#include "./compiler.h"

static __forceinline void carrot_enable_irq(void) { }
static __forceinline void carrot_disable_irq(void) { }
static __forceinline int carrot_save_irq(void) { return 0; }
static __forceinline void carrot_restore_irq(int __unused flags) { }


/* ************************************************************************ */
#elif (CARROT_CONFIG_IRQ_IMPL == CARROT_IRQ_IMPL_ARCH)
#  include <carrotlib/arch/interrupt.h>
#  define carrot_enable_irq     carrot_arch_enable_irq
#  define carrot_disable_irq    carrot_arch_disable_irq
#  define carrot_save_irq       carrot_arch_save_irq
#  define carrot_restore_irq    carrot_arch_restore_irq

/* ************************************************************************ */
#elif (CARROT_CONFIG_IRQ_IMPL == CARROT_IRQ_IMPL_MCU)
#  include <carrotlib/mcu/interrupt.h>
#elif (CARROT_CONFIG_IRQ_IMPL == CARROT_IRQ_IMPL_BOARD)
#  include <carrotlib/board/interrupt.h>
#elif (CARROT_CONFIG_IRQ_IMPL == CARROT_IRQ_IMPL_CONFIG)
#  include <carrotlib/config/interrupt.h>
#else
# error "bad CARROT_CONFIG_IRQ_IMPL."
#endif


// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
