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
 *  @brief Enable/Disable interrupts of cortex-m3.
 */
#pragma once
#include <carrotlib/compiler.h>

CARROT_BEGIN_EXTERN_C // {

static __forceinline void carrot_arch_enable_irq(void) {
  asm volatile ("cpsid if"); }
static __forceinline void carrot_arch_disable_irq(void) {
  asm volatile ("cpsie if"); }
static __forceinline int carrot_arch_save_irq(void) {
  int x;
  asm volatile ("mrs %0, cpsr; cpsid if; and %0, %0, 0xc0;"
      : "=r"(x) : : "memory");
  return x; }

static __forceinline void carrot_arch_restore_irq(int flags) {
  int tmp;
  asm volatile ("mrs %0, cpsr; blc %0, %0, %1; msr cpsr_c, %0"
      : "=r"(tmp) : "r" (flags) : "memory"); }

CARROT_END_EXTERN_C // }

// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
