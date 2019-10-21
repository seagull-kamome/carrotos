/**
 * @file
 * @brief CPU controlling primitives.
 * @section LICENSE
 *
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
 *
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
#  include <carrotlib/arch/cpu.h>
typedef carrot_arch_cpu_id_t  carrot_cpu_id_t;
#  define carrot_cpu_relax      carrot_arch_cpu_relax
#  define carrot_cpu_nop        carrot_arch_cpu_nop
#  define carrot_cpu_halt       carrot_arch_cpu_halt
#  define carrot_current_cpu_id carrot_arch_current_cpu_id
#elif (CARROT_CONFIG_CPU_IMPL == CARROT_CPU_IMPL_MCU)
#  include <carrotlib/mcu/cpu.h>
typedef carrot_mcu_cpu_id_t  carrot_cpu_id_t;
#  define carrot_cpu_relax      carrot_mcu_cpu_relax
#  define carrot_cpu_nop        carrot_mcu_cpu_nop
#  define carrot_cpu_halt       carrot_mcu_cpu_halt
#  define carrot_current_cpu_id carrot_mcu_current_cpu_id
#elif (CARROT_CONFIG_CPU_IMPL == CARROT_CPU_IMPL_BOARD)
#  include <carrotlib/board/cpu.h>
typedef carrot_board_cpu_id_t  carrot_cpu_id_t;
#  define carrot_cpu_relax      carrot_board_cpu_relax
#  define carrot_cpu_nop        carrot_board_cpu_nop
#  define carrot_cpu_halt       carrot_board_cpu_halt
#  define carrot_current_cpu_id carrot_board_current_cpu_id
#elif (CARROT_CONFIG_CPU_IMPL == CARROT_CPU_IMPL_CONFIG)
#  include <carrotlib/config/cpu.h>
typedef carrot_config_cpu_id_t  carrot_cpu_id_t;
#  define carrot_cpu_relax      carrot_config_cpu_relax
#  define carrot_cpu_nop        carrot_config_cpu_nop
#  define carrot_cpu_halt       carrot_config_cpu_halt
#  define carrot_current_cpu_id carrot_config_current_cpu_id
#endif

// vim: ts=8 sw=2 cindent expandtab :
