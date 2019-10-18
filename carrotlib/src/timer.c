/**
 * @file
 * @brief Implementations of software timer.
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

#include <carrotlib/config.h>
#include <carrotlib/compiler.h>
#include <carrotlib/list.h>
#include <carrotlib/atomic.h>
#include <carrotlib/spinlock.h>
#include <carrotlib/time.h>
#include <carrotlib/timer.h>

/* ************************************************************************ */

#if !defined(CARROT_CONFIG_TIMER_IMPL)
#  error "No defined CARROT_CONFIG_TIMER_IMPL."
#elif (CARROT_CONFIG_TIMER_IMPL == CARROT_TIMER_IMPL_ARCH)
#  include <carrotlib/arch/timer.h>
#  define carrot_clock_start_hw_timer   carrot_arch_clock_start_hw_timer
#  define carrot_clock_get_hw_elapsed   carrot_arch_clock_get_hw_elapsed
#  define carrot_clock_set_hw_alarm     carrot_arch_clock_set_hw_alarm
#  define CARROT_CLOCK_HW_HAS_ALARM     CARROT_ARCH_CLOCK_HW_HAS_ALARM
#elif (CARROT_CONFIG_TIMER_IMPL == CARROT_TIMER_IMPL_MCU)
#  include <carrotlib/mcu/timer.h>
#  define carrot_clock_start_hw_timer   carrot_mcu_clock_start_hw_timer
#  define carrot_clock_get_hw_elapsed   carrot_mcu_clock_get_hw_elapsed
#  define carrot_clock_set_hw_alarm     carrot_mcu_clock_set_hw_alarm
#  define CARROT_CLOCK_HW_HAS_ALARM     CARROT_MCU_CLOCK_HW_HAS_ALARM
#elif (CARROT_CONFIG_TIMER_IMPL == CARROT_TIMER_IMPL_BOARD)
#  include <carrotlib/board/timer.h>
#  define carrot_clock_start_hw_timer   carrot_board_clock_start_hw_timer
#  define carrot_clock_get_hw_elapsed   carrot_board_clock_get_hw_elapsed
#  define carrot_clock_set_hw_alarm     carrot_board_clock_set_hw_alarm
#  define CARROT_CLOCK_HW_HAS_ALARM     CARROT_BOARD_CLOCK_HW_HAS_ALARM
#elif (CARROT_CONFIG_TIMER_IMPL == CARROT_TIMER_IMPL_CONFIG)
#  include <carrotlib/config/timer.h>
#else
#  error "Bad CARROT_CONFIG_TIMER_IMPL."
#endif

/* ************************************************************************ */

CARROT_BEGIN_EXTERN_C // {

/* ************************************************************************ */

static struct {
  carrot_spinlock_t lck;
  struct carrot_timespec now;
  struct carrot_timespec last_now;
  struct carrot_timespec alarm_time;

  carrot_list_head_t timers;
} cnxt = {
  .lck = CARROT_SPINLOCK_INITIALIZER,
  .now = { .tv_sec = 0, .tv_nsec = 0 },
  .last_now = { .tv_sec = 0, .tv_nsec = 0 },
};


/* ************************************************************************ */


void carrot_clock_initialize(void) {
  carrot_list_init(&cnxt.timers);
  carrot_clock_start_hw_timer();
}




__nonnull((1)) void carrot_clock_get_system_time(struct carrot_timespec *ts) {
  unsigned int nretries = 2;

  int flags = carrot_spinlock_save_irq(&cnxt.lck);
  while (! carrot_clock_get_hw_elapsed(ts) && --nretries)
    carrot_spinlock_restore_irq(flags, &cnxt.lck);

  if (nretries) {
    ts->tv_sec += cnxt.now.tv_sec;
    ts->tv_nsec += cnxt.now.tv_nsec;
    carrot_timespec_normalize(ts);
    cnxt.last_now = *ts;
  } else {
    *ts = cnxt.last_now;
  } 
  carrot_spinlock_restore_irq(flags, &cnxt.lck);
}


#if defined(CARROT_CLOCK_HW_HAS_ALARM)
static void update_hw_alarm(
    struct carrot_timespec const*now, struct carrot_timespec const* ts) {
  unsigned int dly_us;
  if (now->tv_sec == ts->tv_sec) {
    dly_us = (unsigned int)(ts->tv_nsec - now->tv_nsec) / 1000000u;
  } else {
    dly_us = (unsigned int)(1000000000l - now->tv_nsec + ts->tv_nsec) / 1000u;
    carrot_time_t const dly_sec = ts->tv_sec - now->tv_sec;
    if (dly_sec <= (carrot_time_t)((unsigned int)-1 - dly_us) / 1000000u) {
      dly_us += (unsigned int)dly_sec * 1000000u;
    } else {
      dly_us = (unsigned int)-1;
    }
  }
  carrot_clock_set_hw_alarm(dly_us);
}
#else
#  define update_hw_alarm(x, y)  do { } while (0)
#endif



void carot_clock_give_tick(struct carrot_timespec const* ts) {
  int flags = carrot_spinlock_save_irq(&cnxt.lck);

  // Update current time.
  struct carrot_timespec now = cnxt.now;
  now.tv_nsec = cnxt.now.tv_nsec + ts->tv_nsec;
  now.tv_sec = cnxt.now.tv_sec + ts->tv_sec + now.tv_nsec / CARROT_NSECS_IN_SRC;
  now.tv.nsec %= CARROT_NSECS_IN_SEC;
  cnxt.now = now;
  cnxt.last_now = now;

  // Callback exhausted timers.
  while (true) {
    if (carrot_list_empty(&cnxt.timers)) break;

    struct carrot_timer *x = (struct carrot_timer*)cnxt.timers.next;
    if (now.tv_sec < x->alarm_time.tv_sec
        || (now.tv_sec == x->alarm_time.tv_sec
          && now.tv_nsec + 200000ul < x->alarm_time.tv_nsec)) {
      update_hw_alarm(&now, &x->alarm_time);
      break;
    }

    // Callback
    carrot_list_remove(&x->head);
    carrot_spinlock_restore_irq(flags, &cnxt.lck);
    x->callback(x);
    flags = carrot_spinlock_save_irq(&cnxt.lck);
  }



  carrot_spinlock_restore_irq(flags, &cnxt.lck);
}



/* ************************************************************************ */


__nonnull((2, 3)) void carrot_timer_initialize(
    unsigned int delay_ms,
    void __nonnull((1)) (*cb)(struct carrot_timer *),
    struct carrot_timer *it) {
  carrot_list_init(&it->head);
  it->delay_ms = delay_ms;
  it->callback = cb;
}



__nonnull((1)) void carrot_timer_add_timer(struct carrot_timer *it) {
  assert(it->callback != NULL);

  int flags = carrot_spinlock_save_irq(&cnxt.lck);

  for (carrot_list_head_t *i = cnxt.timers.next; ; i = i->next) {
    if (i == &cnxt.timers
        || carrot_timespec_compare(&((struct carrot_timer*)i)->alarm_time,
                                   &it->alarm_time) >= 0) {
      carrot_list_add_tail(i, &it->head);

      if (carrot_list_is_first(&cnxt.timers, &it->head))
        update_hw_alarm(&cnxt.now, &it->alarm_time);


      carrot_spinlock_restore_irq(flags, &cnxt.lck);
      return ;
    }
  }
  __unreachable();
}


__nonnull((1)) void carrot_timer_del_timer(struct carrot_timer *it) {
  int flags = carrot_spinlock_save_irq(&cnxt.lck);
  if (cnxt.timers.next == &it->head)
    update_hw_alarm(&cnxt.now, &((struct carrot_timer*)it->head.next)->alarm_time);

  carrot_list_remove_and_init(&it->head);

  carrot_spinlock_restore_irq(flags, &cnxt.lck);
}



/* ************************************************************************ */


__nonnull((1)) void carrot_timer_start_alarm(struct carrot_timer *it) {
  assert(it->delay_ms > 0);
  assert(it->delay_ms <= CARROT_TIMER_MAX_DELAY_MS);
  assert(it->callback != NULL);
  assert(carrot_list_empty(&it->head));  // it must stopped.

  carrot_clock_get_system_time(&it->alarm_time);
  carrot_timespec_add_nsec(it->delay_ms * (uint32_t)1000000l,
      &it->alarm_time);
  carrot_timer_add_timer(it);
}



/* ************************************************************************ */

static void carrot_cyclic_cb(struct carrot_timer *it) {
  struct carrot_timespec now;
  carrot_clock_get_system_time(&now);

  unsigned int n = 0;
  do {
    carrot_timespec_add_nsec(it->delay_ms * (uint32_t)1000000l,
                             &it->alarm_time);
    if (n != CARROT_TIMER_MAX_OVERRUN)
      ++n;
  } while (carrot_timespec_compare(&now, &it->alarm_time) >= 0);

  struct carrot_cyclic_timer *p = (struct carrot_cyclic_timer*)it;
  if (! p->callback(n - 1, p))
    carrot_timer_add_timer(it);
}


__nonnull((2,3)) void carrot_timer_init_cyclic(
    unsigned int cycle_ms,
    int __nonnull((2)) (*cb)(unsigned int, struct carrot_cyclic_timer *),
    struct carrot_cyclic_timer *it) {
  it->callback = cb;
  carrot_timer_initialize(cycle_ms, carrot_cyclic_cb, &it->base);
}



__nonnull((1))
void carrot_timer_cancel_cyclic(struct carrot_cyclic_timer *timer) {
}


CARROT_END_EXTERN_C // }


// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
