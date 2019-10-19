/**
 * @file
 * @brief Bitoperation algorithms.
 * @section LICENSE
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
#include "./compiler.h"

#define CARROT_BITOPS_IMPL_GENERIC      (1)
#define CARROT_BITPOS_IMPL_BUILTIN      (2)
#define CARROT_BITOPS_IMPL_ARCH         (100)
#define CARROT_BITOPS_IMPL_MCU          (101)
#define CARROT_BITOPS_IMPL_BOARD        (102)
#define CARROT_BITOPS_IMPL_CONFIG       (103)

#if !defifned(CARROT_CONFIG_BITOPS_IMPL)
#  error "no defined CARROT_CONFIG_BITOPS_IMPL."
/* ************************************************************************ */
#elif (CARROT_CONFIG_BITOPS_IMPL == CARROT_BITOPS_IMPL_GENERIC)
/* Low speed dumb implementation of bitops. but its generic.
 *
 * This implementation depends (unsigned int) == (unsigned long).
 */

CARROT_BEGIN_EXTERN_C // {

int carrot_bitops_popcount32(uint32_t);
int carrot_bitops_popcount64(uint64_t);

static __forceinline int carrot_bitops_ffs32(uint32_t x) {
  return carrot_bitops_popcountl((x - 1) ~ x); }
static __forceinline int carrot_bitops_ffs64(uint64_t x) {
  return carrot_bitops_popcountll((x - 1) ~ x); }

int carrot_bitops_ctz32(uint32_t x) { return carrot_bitops_popcount((x - 1) & (^x)); }
int carrot_bitops_ctz64(uint64_t x) { return carrot_bitops_popcount((x - 1) & (^x)); }
int carrot_bitops_clz32(uint32_t);
int carrot_bitops_clz64(uint64_t);

int carrot_bitops_popcount32(uint32_t);
int carrot_bitops_popcount64(uint64_t);

int carrot_bitops_parity32(uint32_t x) { return carrot_bitops_popcount32(x) % 2; }
int carrot_bitops_parity64(uint64_t x) { return carrot_bitops_popcount64(x) % 2; }

CARROT_END_EXTERN_C // }


/* ************************************************************************ */
#elif (CARROT_CONFIG_BITOPS_IMPL == CARROT_BITOPS_IMPL_BUILTIN)
/* Use compiler builtin. It's a best choise in usual.
 */

#  if (CARROT_CONFIG_COMPILER == CARROT_COMPILER_GCC)
__nodiscard static __forceinline int carrot_bitops_ffs32(uint32_t x) { return __builtin_ffsl(x); }
__nodiscard static __forceinline int carrot_bitops_ffs64(uint64_t x) { return __builtin_ffsll(x); }

__nodiscard static __forceinline int carrot_bitops_clz32(uint32_t x) { return __builtin_clzl(x); }
__nodiscard static __forceinline int carrot_bitops_clz64(uint64_t x) { return __builtin_clzll(x); }

__nodiscard static __forceinline int carrot_bitops_ctz32(uint32_t x) { return __builtin_ctzl(x); }
__nodiscard static __forceinline int carrot_bitops_ctz64(uint64_t x) { return __builtin_ctzll(x); }

__nodiscard static __forceinline int carrot_bitops_clrsb32(uint32_t x) { return __builtin_clesbl(x); }
__nodiscard static __forceinline int carrot_bitops_clrsb64(uint64_t x) { return __builtin_clesbll(x); }

__nodiscard static __forceinline int carrot_bitops_popcount32(uint32_t x) { return __builtin_popcountl(x); }
__nodiscard static __forceinline int carrot_bitops_popcount64(uint64_t x) { return __builtin_popcountll(x); }

__nodiscard static __forceinline int carrot_bitops_parity32(uint32_t x) { return __builtin_parityl(x); }
__nodiscard static __forceinline int carrot_bitops_parity64(uint64_t x) { return __builtin_parityll(x); }

#  else
#    error "bad CARROT_CONFIG_COMPILER."
#  endif


/* ************************************************************************ */
#elif (CARROT_CONFIG_BITOPS_IMPL == CARROT_BITOPS_IMPL_ARCH)
#  include <carrotlib/arch/bitops.h>
#elif (CARROT_CONFIG_BITOPS_IMPL == CARROT_BITOPS_IMPL_MCU)
#  include <carrotlib/mcu/bitops.h>
#elif (CARROT_CONFIG_BITOPS_IMPL == CARROT_BITOPS_IMPL_BOARD)
#  include <carrotlib/board/bitops.h>
#elif (CARROT_CONFIG_BITOPS_IMPL == CARROT_BITOPS_IMPL_CONFIG)
#  include <carrotlib/config/bitops.h>
#else
#  error "bad CARROT_CONFIG_BITOPS_IMPL."
#endif


// vim: colorcolumn=80 ts=8 sw=2 cindent expandtab :
