/*
 * MIT License
 *
 * Copyright (c) 2019 Hattori, Hiroki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/** @file
 *  @brief Interface of atomic operation
 */
#pragma once


#define CARROT_ATOMIC_IMPL_NONE         (0 /* Dummy impl */)
#define CARROT_ATOMIC_IMPL_STD          (1 /* Use stdatomic */)
#define CARROT_ATOMIC_IMPL_ARCH         (100 /* Use carrotlib/arch */)
#define CARROT_ATOMIC_IMPL_MCU          (101 /* Use carrotlib/mcu */)
#define CARROT_ATOMIC_IMPL_BOARD        (102 /* Use carrotlib/board */)
#define CARROT_ATOMIC_IMPL_CONFIG       (103 /* Use carrotlib/port */)


#include <carrotlib/config.h>

#if !defined(CARROT_CONFIG_ATOMIC_IMPL)
#  error "CARROT_CONFIG_ATOMIC_IMPL not defined"
/* ************************************************************************ */
#elif (CARROT_CONFIG_ATOMIC_IMPL == CARROT_ATOMIC_IMPL_NONE)

#  if defined(CARROT_CONFIG_SMP) && CARROT_CONFIG_SMP
#    warning "using CARROT_ATOMIC_IMPL_NONE but no CARROT_CONFIG_SMP defined"
#  else
#    define __atomic
#    define carrot_atomic_fence()        do { } while (0)
#    define carrot_atomic_read(ptr) (*(ptr))
#    define carrot_atomic_write(ptr, val) do { *(ptr) = (val); } while (0)

#    define carrot_atomic_cas(object, expected, desired) ( \
    (*(object) == *(expected)) \
      ? ((*(object) = (desired)), true) \
      : ((*(expected) = *(object)), false) )
#  endif


/* ************************************************************************ */
#elif (CARROT_CONFIG_ATOMIC_IMPL == CARROT_ATOMIC_IMPL_GENERIC)

#  include <stdatomic.h>

#  define __atomic
#  define carrot_atomic_fence() do { atomic_thread_fence(memory_order_seq_cst); } while (0)
#  define carrot_atomic_read(ptr) do { atomic_load((ptr), (val)); } while (0)
#  define carrot_atomic_write(ptr, val)  do { atomic_store((ptr), (val)); } while (0)

#  define carrot_atomic_cas(object, expected, desired) \
  atomic_compare_exchange_strong((object), (expected), (desired(, memory_order_seq_cst))

/* ************************************************************************ */
#elif (CARROT_CONFIG_ATOMIC_IMPL == CARROT_ATOMIC_IMPL_ARCH)
#  include <carrotlib/arch/atomic.h>
#elif (CARROT_CONFIG_ATOMIC_IMPL == CARROT_ATOMIC_IMPL_MCU)
#  include <carrotlib/mcu/atomic.h>
#elif (CARROT_CONFIG_ATOMIC_IMPL == CARROT_ATOMIC_IMPL_BOARD)
#  include <carrotlib/board/atomic.h>
#elif (CARROT_CONFIG_ATOMIC_IMPL == CARROT_ATOMIC_IMPL_CONFIG)
#  include <carrotlib/config/atomic.h>
#else
#  error "Bad CARROT_CONFIG_ATOMIC_IMPL"
#endif


// vim: ts=8 sw=2 cindent expandtab :
