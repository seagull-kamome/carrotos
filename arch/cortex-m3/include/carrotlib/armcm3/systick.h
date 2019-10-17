/* @file
 * @brief Peripheral driver of Cortex-M3 System Timer
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

#define CARROT_ARMCM3_SYSTICK_MAX_RELOADVAL  (0x00fffffful)


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



static __forceinline void carrot_armcm3_systick_configure(
    bool enabled,
    uint32_t reload,
    bool use_external_clock,
    bool use_interrupt) {
  carrot_armcm3_systick_disable();
  carrot_armcm3_systick_set_reload_counter(reload);
  CARROT_ARMCM3_SYSTICK->csr
    = (ARMCM3_SYSTICK->csr & (0xfffffff8))
    | (use_external_clock? (1ul << 2) : 0)
    | (use_interrupt? (1ul << 1) : 0)
    | (enabled? 1ul : 0);
}



// vim: ts=8 sw=2 cindent expandtab :
