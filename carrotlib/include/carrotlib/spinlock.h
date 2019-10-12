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
 *  @brief Interface/Implements to spinlock.
 */
#include <carrotlib/config.h>

#define CARROT_SPINLOCK_IMPL_NONE        (0)
#define CARROT_SPINLOCK_IMPL_ATOMIC      (1)
#define CARROT_SPINLOCK_IMPL_ARCH        (100)
#define CARROT_SPINLOCK_IMPL_MCU         (101)
#define CARROT_SPINLOCK_IMPL_BOARD       (102)
#define CARROT_SPINLOCK_IMPL_CONFIG      (103)


#if !defined(CARROT_CONFIG_SPINLOCK_IMPL)
#  error "CARROT_CONFIG_SPINLOCK_IMPL not defined."
/* ************************************************************************ */
#elif (CARROT_CONFIG_SPINLOCK_IMPL == CARROT_SPINLOCK_IMPL_NONE)
/*
 * Dummy implementation for single processor system.
 */
#  if defined(CARROT_CONFIG_SMP) && (CARROT_CONFIG_SMP)
#    error "CARROT_CONFIG_SMP needs valid spinlock."
#  endif

#include "./interrupt.h"

typedef struct { } __unused carrot_spinlock_t;

#define CARROT_SPINLOCK_INITIALIZER     {}

#  define carrot_spinlock_init(ptr)      do { } while (0)
#  define carrot_spinlock_save_irq(ptr)  (carrot_save_irq())
#  define carrot_spinlock_restore_irq(flag, ptr) do { \
  carrot_restore_irq(flag); } while (0)


/* ************************************************************************ */
#elif (CARROT_CONFIG_SPINLOCK_IMPL == CARROT_SPINLOCK_IMPL_ATOMIC)
/*
 *
 */
#  include "./compiler.h"
#  include "./atomic.h"
#  include "./interrupt.h"


typedef struct { int do_not_touch; } carrot_spinlock_t;
#  define CARROT_SPINLOCK_INITIALIZER   { 0 }

CARROT_BEGIN_EXTERN_C

__nonnull((1))
static __forceinline void carrot_spinlock_init(carrot_spinlock_t *lck) {
  *lck->do_not_touch = 0;
}


__nodiscard __nonnull((1))
static __forceinline int carrot_spinlock_save_irq(carrot_spinlock_t *lck) {
  int flags = carrot_save_irq();

  int expected = 0;
  while (! carrot_atomic_cas(&lck->do_not_touch, &expected, 1)) {
    carrot_restore_irq(flags);
    expected = 0;
    flags = carrot_save_irq();
  }

  return flags;
}


__nonnull((2))
static __forceinline void carrot_spinlock_restore_irq(
    int flags,
    carrot_spinlock_t *lck) {
  carrot_atomic_write(&lck->do_not_touch, 0);
  carrrot_restore_irq(flags);
}

CARROT_END_EXTERN_C

/* ************************************************************************ */
#elif (CARROT_CONFIG_SPINLOCK_IMPL == CARROT_SPINLOCK_IMPL_ARCH)
#  include <carrotlib/arch/spinlock.h>
#elif (CARROT_CONFIG_SPINLOCK_IMPL == CARROT_SPINLOCK_IMPL_MCU)
#  include <carrotlib/mcu/spinlock.h>
#elif (CARROT_CONFIG_SPINLOCK_IMPL == CARROT_SPINLOCK_IMPL_BOARD)
#  include <carrotlib/board/spinlock.h>
#elif (CARROT_CONFIG_SPINLOCK_IMPL == CARROT_SPINLOCK_IMPL_CONFIG)
#  include <carrotlib/config/spinlock.h>
#else
#  error "Bad CARROT_CONFIG_SPINLOCK_IMPL"
#endif

// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
