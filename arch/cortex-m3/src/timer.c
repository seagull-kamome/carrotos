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

static uint32_t last_loadval;


void carrot_arch_clock_start_hw_timer(void) {
  last_loadval = CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL;
  carrot_armcm3_systick_configure(
      true, CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL,
      (CARROT_CONFIG_ARMCM3_SYSTICK_CLKSRC? true : false),
      true);
}


bool carrot_arch_clock_get_hw_elapsed(struct carrot_timespec *ts) {
  // FIXME:
}


void carrot_arch_clock_set_hw_alarm(unsigned int delay_usec) {
  // FIXME:
}

void carrot_armcm3_systick_isr(void) {
  carrot_clock_give_tick();
}



