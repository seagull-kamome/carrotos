/* \file arch/armcm3/include/carrot_armcm3/systick.h
 * Peripheral driver of Cortex-M3 System Timer
 */
/*
 * Copyright (c) 2018, Hattori Hiroki All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the carrotos developper team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <carrotlib/compiler.h>

struct carrot_armcm3_systick {
  uint32_t volatile csr;
  uint32_t volatile rvr;
  uint32_t volatile cvr;
  uint32_t volatile const calib;
};

#define CARROT_ARMCM3_SYSTICK         ((struct armcm3_systick*)0xe000e010ul)


__nodiscard static __forceinline bool carrot_armcm3_systick_asserted(void) {
  return (CARROT_ARMCM3_SYSTICK->csr & (1ul << 16))? true : false;
}
static __forceinline void carrot_armcm3_systick_enable(void) {
  ARMCM3_SYSTICK->csr |= 1ul; }
static __forceinline void carrot_armcm3_systick_disable(void) {
  ARMCM3_SYSTICK->csr &= ~1ul; }

__nodiscard static __forceinline uint32_t carrot_armcm3_get_systick_reload_counter(void) {
  return ARMCM3_SYSTICK->rvr & 0x00fffffful; }
static __forceinline void carrot_armcm3_systick_set_reload_counter(
    uint32_t reload) {
  ARMCM3_SYSTICK->rvr = (ARMCM3_SYSTICK->rvr & 0xff000000ul) + reload; }


static inline uint32_t carrot_armcm3_systick_current_value(void) {
  uint32_t x = ARMCM3_SYSTICK->cvr;
  uint32_t y;
  while ((y = ARMCM3_SYSTICK->cvr) != x) x = y;
  return y & 0x00fffffful;;
}



static __forceinline void carrot_armcm3_systick_configuer(
    bool enabled,
    uint32_t reload,
    bool use_external_clock,
    bool use_interrupt) {
  armcm3_systick_disable();
  armcm3_systick_set_reload_counter(reload);
  ARMCM3_SYSTICK->csr
    = (ARMCM3_SYSTICK->csr & (0xfffffff8))
    | (use_external_clock? (1ul << 2) : 0)
    | (use_interrupt? (1ul << 1) : 0)
    | (enabled? 1ul : 0);
}



// vim: ts=8 sw=2 cindent expandtab :
