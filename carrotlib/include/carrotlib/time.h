

#pragma once
#include <stdint.h>
#include <stdbool.h>

#include <carrotlib/compiler.h>

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


