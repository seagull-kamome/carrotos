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
 *  @brief HAL of cortex-m3 SCB.
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <carrotlib/compiler.h>

CARROT_BEGIN_EXTERN_C

struct carrot_armcm3_scb {
  uint32_t const cpuid;         // +0000
  uint32_t volatile icsr;       // +0004
  uint32_t volatile vtor;       // +0008
  uint32_t volatile aircr;      // +000c
  uint32_t volatile scr;        // +0010
  uint32_t volatile ccr;        // +0014
  union {
    uint32_t volatile shpr[3];    // +0018
    uint8_t volatile shp[16];
  };
  uint32_t volatile shprs;      // +0024
  union {
    uint32_t volatile cfsr;      // +0028
    struct __packed {
      uint8_t volatile mmsr;
      uint8_t volatile bfsr;
      uint8_t volatile ufsr;
      uint8_t volatile reserved_1;
    };
  };
  uint32_t volatile hfsr;       // +002c
  uint32_t volatile reserved_2; // +0030
  uint32_t volatile mmar;       // +0034
  uint32_t volatile bfar;       // +0038
  uint32_t volatile afsr;       // +003c
};

#define CARROT_ARMCM3_SCB     ((struct carrot_armcm3_scb*)0xe000ed00ul)

/* ************************************************************************ */

static __forceinline void carrot_armcm3_scb_set_pendsv_pending(bool b) { 
  CARROT_ARMCM3_SCB->icsr = 1ul << (b? 28 : 27); }
__nodiscard static __forceinline bool carrot_armcm3_scb_pendsv_pending(void) {
  return (CARROT_ARMCM3_SCB->icsr & (1ul << 28))? true : false; }


static __forceinline void carrot_armcm3_scb_set_systick_pending(bool b) {
  CARROT_ARMCM3_SCB->icsr = 1ul << (b? 26 : 25); }
__nodiscard static __forceinline bool carrot_armcm3_scb_systick_pending(void) {
  return (CARROT_ARMCM3_SCB->icsr & (1ul << 26))? true : false; }

/* ************************************************************************ */

__nodiscard static __forceinline bool carrot_armcm3_isr_pending(void) {
  return (ARMCM3_SCB->icsr & (1ul << 22))? true : false; }
__nodiscard static __forceinline uint8_t carrot_armcm3_vector_pending(void) {
  return (uint8_t)((ARMCM3_SCB->icsr >> 12) & 0x3ful;) }
__nodiscard static __forceinline bool carrot_armcm3_isr_return_to_base(void) {
  return (ARMCM3_SCB->icsr & (1ul << 11))? true : false; }
__nodiscard static __forceinline uint8_t carrot_armcm3_current_exception(void)
  { return (uint8_t)(ARMCM3_SCB->icsr & 0xfful); }
static void __forceinline carrot_armcm3_scb_set_isr_vector(void* addr) {
  ARMCM3_SCB->vtor = (uint32_t)addr;
  asm volatile("dsb; isb;"); }
static void* __forceinline carrot_armcm3_scb_isr_vector(void) {
  return (void*)ARMCM3_SCB->vtor; }

/* ************************************************************************ */

static void __forceinline carrot_armcm3_reset_system(void) {
  asm volatile("dsb;");
  ARMCM3_SCB->aircr
    = 0x05fa0000ul // key
    | (ARMCM3_SCB->aircr & 0x00000700ul) // save prigroup
    | 0x000004ul; // sysresreq
  asm volatile("dsb;");
  for (;;) asm volatile("nop;"); }


CARROT_END_EXTERN_C

// vim: ts=8 sw=2 cindent expandtab :
