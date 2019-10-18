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
 */
#pragma once
#include <assert.h>
#include <carrotlib/config.h>
#include <carrotlib/time.h>
#include <carrotlib/timer.h>
#include <carrotlib/interrupt.h>

#include <carrotlib/arch/timer.h>
#include <carrotlib/armcm3/systick.h>

/* ************************************************************************ */

#if !defined(CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ)
#  error "No CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ defined."
#endif
#if !defined(CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC)
#  error "No CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC defined."
#elif (CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC != CARROT_ARMCM3_SYSTICK_CLKSRC_SYS) \
   && (CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC != CARROT_ARMCM3_SYSTICK_CLKSRC_EXT)
#  error "Bad CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC."
#endif

/* ************************************************************************ */

static struct {
  bool elapsed;
  uint32_t last_loadval;
  uint32_t remains;
} volatile cnxt;


void carrot_arch_clock_start_hw_timer(void) {
  cnxt.last_loadval = CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL;
  cnxt.remains = 0;
  carrot_armcm3_systick_configure(
      true, CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL,
      (CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC? false : true),
      true);
}


static uint32_t calc_ts(struct carrot_timespec *ts, uint32_t n) {
  ts->tv_sec = n / CARROT_ARMCM3_SYSTICK_INFREQ_HZ;
  n %= CARROT_ARMCM3_SYSTICK_INFREQ_HZ;

  if (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ % 1000000000ul == 0) {
    ts->tv_nsec = n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000000ul);
    n %= CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC / 1000000000ul;
  } else if (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ % 1000000ul == 0) {
    ts->tv_nsec = n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000ul) * 1000ul;
    n = n % (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000ul) * 1000ul
      + cnxt.remains;
    ts->tv_nsec += n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000ul);
    n %= CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000ul;
  } else {
    assert(CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ % 10 == 0);
    ts->tv_nsec = 0;
    for (unsigned int i = 0; i < 8; ++i) {
      ts->tv_nsec += n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10);
      ts->tv_nsec *= 10;
      n %= CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10;
    }
    n += cnxt.remains;
    ts->tv_nsec += n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10);
    n %= CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10;
  }
  return n;
}


bool carrot_arch_clock_get_hw_elapsed(struct carrot_timespec *ts) {
  // Assuming that interrupt are disabled.
  uint32_t n = carrot_armcm3_systick_current_value();
  if (cnxt.elapsed || CARROT_ARMCM3_SYSTICK->csr & (1ul << 16)) {
    cnxt.elapsed = true;
    return false; // it should retry.
  }
  assert(n <= cnxt.last_loadval);
  calc_ts(n, ts);
  return true;
}


void carrot_arch_clock_set_hw_alarm(struct timespec* dlyts) {
  if (cnxt.elapsed) return;

  int flags = carrot_save_irq();
  CARROT_ARMCM3_SYSTICK->csr = CARROT_ARMCM3_SYSTICK_CLKSRC? 0x04 : 0ul; // disable in temporary.
  if (CARROT_ARMCM3_SYSTICK->csr & (1ul << 16)) {
    CARRT_ARMCM3_SYSTICK->csr = CARROT_ARMCM3_SYSTICL_CLKSRC? 0x05 : 0x01; // start again
    cnxt.elapsed = true;
    return ;
  }

  uint32_t n = carrot_armcm3_systick_current_value();
  struct carrot_timespec ts;
  assert(n <= cnxt.last_loadval);
  calc_ts(cnxt.last_loadval - n, &ts);


}


void carrot_armcm3_systick_isr(void) {
  carrot_disable_irq();
  struct carrot_timespec ts;
  cnxt.remains = calc_ts(&ts, cnxt.last_loadval);
  cnxt.last_reloadval = CARROT_ARMCM3_SYSTICK_MAX_READVAL;
  cnxt.elapsed = false;
  carrot_enable_irq();

  carrot_clock_give_tick(&ts);
}


// vim: ts=8 sw=2 cindent expandtab :
