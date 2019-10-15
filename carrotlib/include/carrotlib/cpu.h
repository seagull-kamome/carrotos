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
 *  @brief
 */
#pragma once
#include <carrotlib/compiler.h>
#include <carrotlib/config.h>

#define CARROT_CPU_IMPL_ARCH    (100)
#define CARROT_CPU_IMPL_MCU     (101)
#define CARROT_CPU_IMPL_BOARD   (102)
#define CARROT_CPU_IMPL_CONFIG  (103)

#if !defined(CARROT_CONFIG_CPU_IMPL)
#  error "no defined CARROT_CONFIG_CPU_IMPL."
#elif (CARROT_CONFIG_CPU_IMPL == CARROT_CPU_IMPL_ARCH)
#  include <carrot/arch/cpu.h>
#  define carrot_relax_cpu      carrot_arch_relax_cpu
#  define carrot_cpu_nop        carrot_arch_cpu_nop
#  define carrot_current_cpu_id carrot_arch_current_cpu_id
#elif (CARROT_CONFIG_CPU_IMPL == CARROT_CPU_IMPL_MCU)
#  include <carrot/mcu/cpu.h>
#  define carrot_relax_cpu      carrot_mcu_relax_cpu
#  define carrot_cpu_nop        carrot_mcu_cpu_nop
#  define carrot_current_cpu_id carrot_mcu_current_cpu_id
#elif (CARROT_CONFIG_CPU_IMPL == CARROT_CPU_IMPL_BOARD)
#  include <carrot/board/cpu.h>
#  define carrot_relax_cpu      carrot_board_relax_cpu
#  define carrot_cpu_nop        carrot_board_cpu_nop
#  define carrot_current_cpu_id carrot_board_current_cpu_id
#elif (CARROT_CONFIG_CPU_IMPL == CARROT_CPU_IMPL_CONFIG)
#  include <carrot/config/cpu.h>
#  define carrot_relax_cpu      carrot_config_relax_cpu
#  define carrot_cpu_nop        carrot_config_cpu_nop
#  define carrot_current_cpu_id carrot_config_current_cpu_id
#endif

// vim: ts=8 sw=2 cindent expandtab :
