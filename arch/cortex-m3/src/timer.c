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
#include <assert.h>
#include <carrotlib/config.h>
#include <carrotlib/time.h>
#include <carrotlib/timer.h>
#include <carrotlib/interrupt.h>

#include <carrotlib/arch/timer.h>
#include <carrotlib/armcm3/systick.h>

/* ************************************************************************ */

#if !defined(CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ)
#  error "No CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ defined."
#endif
#if !defined(CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC)
#  error "No CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC defined."
#elif (CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC != CARROT_ARMCM3_SYSTICK_CLKSRC_SYS) \
   && (CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC != CARROT_ARMCM3_SYSTICK_CLKSRC_EXT)
#  error "Bad CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC."
#endif

/* ************************************************************************ */

struct tick2ts {
  struct carrot_timespec ts;
  uint32_t remains;
};

static inline __pure struct tick2ts tick_to_ts(uint32_t n, uint32_t m) {

  struct tick2ts tt;
  tt.ts.tv_sec = n / CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ;
  n %= CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ;

  assert(CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ > 0);
  if (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ == 0) {
    CARROT_CONFIG_FATAL_ERROR_HANDLER();
  } else if (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ % 1000000000ul == 0) {
    tt.ts.tv_nsec = n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000000ul);
    n %= CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC / 1000000000ul;
    n += m;
  } else if (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ % 1000000ul == 0) {
    tt.ts.tv_nsec = n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000ul) * 1000ul;
    n = n % (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000ul) * 1000ul
      + m;
    tt.ts.tv_nsec += n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000000ul);
    n %= CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 1000ul;
  } else {
    assert(CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ % 10 == 0);
    tt.ts.tv_nsec = 0;
    for (unsigned int i = 0; i < 8; ++i) {
      tt.ts.tv_nsec += n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10);
      tt.ts.tv_nsec *= 10;
      n %= CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10;
    }
    n += m;
    tt.ts.tv_nsec += n / (CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10);
    n %= CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ / 10;
  }
  tt.remains = n;
  return tt; // I believe in power of pure! please!! I beg you optimizer!
}


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


bool carrot_arch_clock_get_hw_elapsed(struct carrot_timespec *ts) {
  // Assuming that interrupt are disabled.
  uint32_t n = carrot_armcm3_systick_current_value();
  if (cnxt.elapsed || CARROT_ARMCM3_SYSTICK->csr & (1ul << 16)) {
    cnxt.elapsed = true;
    return false; // it should retry.
  }
  assert(n <= cnxt.last_loadval);
  *ts = tick_to_ts(n, cnxt.remains).ts;
  return true;
}


void carrot_arch_clock_set_hw_alarm(unsigned int delay_us) {
  uint64_t x = (delay_us == (unsigned int)-1)
    ? (uint64_t)CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL + 1
    : (uint64_t)CARROT_CONFIG_ARMCM3_SYSTICK_INFREQ_HZ * delay_us
                    / 1000000ul;

  int flags = carrot_save_irq();
  CARROT_ARMCM3_SYSTICK->csr = CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC? 0x04 : 0ul; // disable in temporary.
  if (cnxt.elapsed || CARROT_ARMCM3_SYSTICK->csr & (1ul << 16)) {
    CARROT_ARMCM3_SYSTICK->csr = CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC? 0x05 : 0x01; // start again
    cnxt.elapsed = true;
    carrot_restore_irq(flags);
    return;
  }
  x -= cnxt.last_loadval - carrot_armcm3_systick_current_value();
  CARROT_ARMCM3_SYSTICK->cvr = (uint32_t)__min(x, (uint64_t)CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL);

  CARROT_ARMCM3_SYSTICK->csr = CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC? 0x05 : 0x01; // start again
  carrot_restore_irq(flags);
}


void carrot_armcm3_systick_isr(void) {
  carrot_disable_irq();
  struct tick2ts tmp = tick_to_ts(cnxt.last_loadval, cnxt.remains);
  cnxt.remains = tmp.remains;
  cnxt.last_loadval = CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL;
  cnxt.elapsed = false;
  carrot_enable_irq();

  carrot_clock_give_tick(&tmp.ts);
}


// vim: ts=8 sw=2 cindent expandtab :
