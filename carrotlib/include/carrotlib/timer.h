/**
 * @file
 * @brief Interface to software timer.
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
 *   - CARROT_CONFIG_TIMER_IMPL
 */

#pragma once
#include <carrotlib/time.h>
#include <carrotlib/compiler.h>

/** \ingroup carrotlib-software-timer
 * @{ */

#define CARROT_TIMER_IMPL_ARCH          (100)
#define CARROT_TIMER_IMPL_MCU           (101)
#define CARROT_TIMER_IMPL_BOARD         (102)
#define CARROT_TIMER_IMPL_CONFIG        (103)


#define CARROT_TIMER_MAX_DELAY_MS  ((uint32_t)(-1) / (uint32_t)1000000l)


CARROT_BEGIN_EXTERN_C // {

/* ************************************************************************ */

/** Initialize software timer.
 *
 *   - Initialize internal state.
 *   - Call CARROT_CONFIG_TIMER_START to start HW timer.
 *   - You need to call this before any timer/clock APIs.
 */
void carrot_clock_initialize(void);



/** Retrieve current time.
 * @param ts    Pointer to timespec. @sa <carrotlib/time.h>
 */
__nonnull((1)) void carrot_clock_get_system_time(struct carrot_timespec *ts);



/** Notice a hardware clock period exeeded.
 * @param ts       Elapsed time from last period.
 *
 * You can call this from any ISR.
 *
 *   - No reentrant.
 */
void carot_clock_give_tick(struct carrot_timespec const* ts);


/* ************************************************************************ */


struct carrot_timer {
  carrot_list_head_t head;
  struct carrot_timespec alarm_time;

  unsigned int delay_ms;
  void (*callback)(struct carrot_timer *timer);
};



/** Initialize a timer.
 * @param delay_ms   A delay time of the timer will trigger.
 * @param cb         Callback function.
 * @param it         Timer object to initialize.
 */
__nonnull((2, 3)) void carrot_timer_initialize(
    unsigned int delay_ms,
    void __nonnull((1)) (*cb)(struct carrot_timer *),
    struct carrot_timer *it);


/** Activate timer.
 * @param it        Timer object to activate.
 */
__nonnull((1)) void carrot_timer_add_timer(struct carrot_timer *it);


/** Deactivate timer.
 * @param it        Timer object to inactivate.
 */
__nonnull((1)) void carrot_timer_del_timer(struct carrot_timer *it);



/* ************************************************************************ */

/** Start a one-shot timer.
 * @param it        Timer object to start.
 *
 * The timer must initialized with carrot_timer_initialize().
 *
 */
__nonnull((1)) void carrot_timer_start_alarm(struct carrot_timer *it);


/** Stop a one-shot timer.
 * @param it        Timer object to stop.
 *
 * It is safe to stop timer that not-started or already-triggered.
 */
__nonnull((1)) static __forceinline void carrot_timer_cancel_alarm(
    struct carrot_timer *it) { carrot_timer_del_timer(it); }


/* ************************************************************************ */

struct carrot_cyclic_timer {
  struct carrot_timer base;
  int (*callback)(unsigned int n_overruns, struct carrot_cyclic_timer *it);
};

#define CARROT_TIMER_MAX_OVERRUN   ((unsigned int)-1)

/** Initialize cyclic timer
 * @param cycle_ms    Cycle period in ms.
 * @param cb          Callback
 * @param it          The timer
 *
 * You donn't need to use carrot_timer_initialize() before call this.
 */
__nonnull((2,3)) void carrot_timer_init_cyclic(
    unsigned int cycle_ms,
    int __nonnull((2)) (*cb)(unsigned int, struct carrot_cyclic_timer *),
    struct carrot_cyclic_timer *it);

/** Start cycle timer.
 * @param it          The cyclic-timer to start.
 */
__nonnull((1)) static __forceinline void carrot_timer_start_cyclic(
    struct carrot_cyclic_timer *it) { carrot_timer_start_alarm(&it->base); }

/** Stop cyclic timer.
 * @param it          The cyclic-timer to stop.
 */
__nonnull((1)) void carrot_timer_cancel_cyclic(
    struct carrot_cyclic_timer *it);


CARROT_END_EXTERN_C // }

/* @} */

// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
