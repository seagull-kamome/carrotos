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
 *  @brief Interface to NVIC.
 */


#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <carrotlib/config.h>

#include "./interrupts.h"
#include "./scb.h"
#include <carrotlib/compiler.h>
#include <carrot_armcm3/target.h>


/* ************************************************************************ */

#if !defined(CARROT_CONFIG_ARMCM3_NVIC_PRIO_BITS)
#  error "CARROT_CONFIG_ARMCM3_NVIC_PRIO_BITS not defined"
#elif (CARROT_CONFIG_ARMCM3_NVIC_PRIO_BITS <= 0) \
   || (CARROT_CONFIG_ARMCM3_NVIC_PRIO_BITS >= 8)
#  error "Bad CARROT_CONFIG_ARMCM3_NVIC_PRIO_BITS"
#endif

/* ************************************************************************ */

struct carrot_armcm3_nvic {
  uint32_t volatile iser[8];
  uint32_t reserved_1[32 - 8];

  uint32_t volatile icer[8];
  uint32_t reserved_2[32 - 8];

  uint32_t volatile ispr[8];
  uint32_t reserved_3[32 - 8];

  uint32_t volatile icpr[8];
  uint32_t reserved_3[32 - 8];

  uint32_t volatile iabr[8];
  uint32_t reserved_4[64 - 8];

  union {
    uint32_t volatile ipr[60];
    uint8_t volatile ip[240];
  };
  uint32_t reserved_5[4 + 64 * 10];

  uint32_t volatile stir;
};


#define CARROT_ARMCM3_NVIC    ((struct carrot_armcm3_nvic*)0xe000e100ul)

static __forceinline void carrot_armcm3_nvic_enable_irq(armcm3_irqn_t n) {
  ARMCM3_NVIC->iser[n / 32] = 1ul << (n % 32); }
static __forceinline void carrot_armcm3_nvic_disable_irq(armcm3_irqn_t n) {
  ARMCM3_NVIC->icer[n / 32] = 1ul << (n % 32); }
__nodiscard static __forceinline int32_t carrot_armcm3_nvic_save_irq(
    armcm3_irqn_t n) {
  uint32_t x = ARMCM3_NVIC->icer[n / 32];
  ARMCM3_NVIC->icer[n / 32] = 1ul << (n % 32);
  return x; }
static __forceinline void carrot_armcm3_nvic_restore_irq(
    armcm3_irqn_t n, uint32_t flgs) {
  ARMCM3_NVIC->iser[n / 32] = flgs & (1ul << (n % 32)); }

/* ************************************************************************ */

static __forceinline void carrot_armcm3_nvic_set_irq_pending(
    armcm3_irqn_t n) {
  ARMCM3_NVIC->ispr[n / 32] = 1ul << (n % 32); }
static __forceinline void carrot_armcm3_nvic_clear_irq_pending(
    armcm3_irqn_t n) { ARMCM3_NVIC->icpr[n / 32] = 1ul << (n % 32); }
__nodiscard static __forceinline bool carrot_armcm3_nvic_is_irq_pending(
    armcm3_irqn_t n) { return (ARMCM3_NVIC->ispr[n / 32] & (1ul << (n % 32)))? true : false; }
__nodiscard static __forceinline bool carrot_armcm3_nvic_is_irq_active(
    armcm3_irqn_t n) { return (ARMCM3_NVIC->iabr[n / 32] & (1ul << (n % 32)))? true : false; }

static inline void carrot_armcm3_nvic_set_irq_priority(
    armcm3_irqn_t n,
    uint8_t prio) {
  if (n < 4) {
    ;
  } else if (n < ARMCM3_MIN_IRQN) {
    ARMCM3_SCB->shp[n - 4] = prio << (8 - ARMCM3_TARGET_NVIC_PRIO_BITS);
  } else {
    ARMCM3_NVIC->ip[n - ARMCM3_MIN_IRQN] = prio << (8 - ARMCM3_TARGET_NVIC_PRIO_BITS);
  }
}

static inline uint8_t carrot_armcm3_nvic_get_irq_priority(
    armcm3_irqn_t n) {
  return
    (n < 4)? 0xffu :
    (n < ARMCM3_MIN_IRQN)? (ARMCM3_SCB[n - 4] >> (8 - ARMCM3_TARGET_NVIC_PRIO_BITS)) :
    (ARMCM3_NVIC->ip[n - ARMCM3_MIN_IRQN] >> (8 - ARMCM3_TARGET_NVIC_PRIO_BITS));
}

// vim: ts=8 sw=2 cindent expandtab :
