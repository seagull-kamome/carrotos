/**
 * @file
 * @brief Avoid toolchain dependency.
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
 */
#pragma once
#include <stddef.h>


#define CARROT_COMPILER_GCC             (1)
#define CARROT_COMPILER_CLANG           (2)



/* ************************************************************************ */
/* Compiler independent shorthands. */

#if defined(__cplusplus)
#  define CARROT_BEGIN_EXTERN_C  extern "C" {
#  define CARROT_END_EXTERN_C    }
#  define __extern_C        extern "C"
#else
#  define CARROT_BEGIN_EXTERN_C
#  define CARROT_END_EXTERN_C
#  define __extern_C
#endif



/* ************************************************************************ */
/* Compiler dependent shorthands. */

#if !defined(CARROT_CONFIG_COMPILER)
#  error "Give configuration value of CARROT_CONFIG_COMPILER"
#elif  (CARROT_CONFIG_COMPILER == CARROT_COMPILER_GCC)

#  include <sys/cdefs.h>

#  define __forceinline  __attribute__((always_inline)) inline
#  define __naked        __attribute__((naked))
//#  define __nonnull(X)   __attribute__((nonnull X))
#  define __nodiscard    __attribute__((warn_unused_result))
#  define __noreturn     __attribute__((noreturn))
//#  define __unused       __attribute__((unused))
//#  define __pure         __attribute__((pure))
//#  define __const        __attribute__((const))
#  define __constrant_p(X)  __builtin_constant_p(X)
#  define __pack         __attribute__((packed))
#  define __align(X)     __attribute__((aligned(X)))
#  define __weak         __attribure__((weak))
#  if __GNUC__ >= 7
#    define __fallthrough  __attribute__((fallthrough))
#  else
#    define __fallthrough
#  endif
//#  define __unreachable()  do { __builtin_unreachable(); } while (0)

#if defined(S_SPRINT_S)
#  undef __asm__
#  define __asm__(X)   do { } while(0)
#endif


#  define __max(a, b) \
  ({ __typeof__(a) _a = (a); \
     __typeof__(b) _b = (b); \
     _a > _b? _a : _b; })
 #  define __min(a, b) \
  ({ __typeof__(a) _a = (a); \
     __typeof__(b) _b = (b); \
     _a < _b? _a : _b; })

//#define offsetof(typ, mbr) __builtin_offsetof(typ, mbr)
#define container_of(ptr, typ, mbr) \
  ((typ*)(char*)(ptr) - offsetof(type, mbr))


#else
#  error "Unknown compiler"
#endif

// vim: ts=8 sw=2 cindent expandtab :

