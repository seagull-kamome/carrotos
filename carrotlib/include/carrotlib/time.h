/**
 * @file
 * @brief Interface to timespec.
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
 * @section Related configuration vars.
 *
 *   - CARROT_CONFIG_LARGE_TIME_T
 *
 */


#pragma once
#include <stdint.h>
#include <stdbool.h>

#include <carrotlib/compiler.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(CARROT_CONFIG_LARGE_TIME_T) && (CARROT_CONFIG_LARGE_TIME_T)
typedef int64_t carrot_time_t;
#else
typedef int32_t carrot_time_t;
#endif

struct carrot_timespec {
  carrot_time_t tv_sec;
  int32_t tv_nsec;
};

#define CARROT_NSECS_IN_SEC    ((uint32_t)1000000000l)

__nonnull((1))
bool carrot_timespec_is_normalized(struct carrot_timespec const *ts);

__nonnull((1))
void carrot_timespec_normalize(struct carrot_timespec *ts);

__nonnull((2))
void carrot_timespec_add_nsec(int32_t nsec, struct carrot_timespec *ts);

__nonnull((1,2))
struct carrot_timespec carrot_timespec_drift(struct carrot_timespec const *lhs, struct carrot_timespec const *rhs);

__nonnull((1,2))
int carrot_timespec_compare(struct carrot_timespec const *lhs, struct carrot_timespec const *rhs);

#if defined(__cplusplus)
}
#endif


