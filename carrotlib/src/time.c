
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <carrotlib/time.h>




__nonnull((1))
bool carrot_timespec_is_normalized(struct carrot_timespec const *ts) {
  return (ts->tv_sec < 0 && ts->tv_nsec > 0)
      || (ts->tv_sec > 0 && ts->tv_nsec < 0)
      || (ts->tv_nsec > 0 && ts->tv_nsec >= CARROT_NSECS_IN_SEC)
      || (ts->tv_nsec < 0 && ts->tv_nsec < -CARROT_NSECS_IN_SEC) ;
}


__nonnull((1))
void carrot_timespec_normalize(struct carrot_timespec *ts) {
  ts->tv_sec += ts->tv_nsec / CARROT_NSECS_IN_SEC;
  ts->tv_nsec %= CARROT_NSECS_IN_SEC;

  if (ts->tv_sec > 0 && ts->tv_nsec < 0) {
    --ts->tv_sec;
    ts->tv_nsec += CARROT_NSECS_IN_SEC;
  } else if (ts->tv_sec < 0 && ts->tv_nsec > 0) {
    ++ts->tv_sec;
    ts->tv_nsec -= CARROT_NSECS_IN_SEC;
  }
}


__nonnull((2))
void carrot_timespec_add_nsec(int32_t nsec, struct carrot_timespec *ts) {
  ts->tv_sec += nsec / CARROT_NSECS_IN_SEC;
  ts->tv_nsec += nsec % CARROT_NSECS_IN_SEC;

  ts->tv_sec += ts->tv_nsec / CARROT_NSECS_IN_SEC;
  ts->tv_nsec %= CARROT_NSECS_IN_SEC;
}


__nonnull((1,2))
struct carrot_timespec carrot_timespec_drift(struct carrot_timespec const *lhs, struct carrot_timespec const *rhs) {
  int32_t r = lhs->tv_nsec - rhs->tv_nsec;
  return (struct carrot_timespec){
    .tv_sec = lhs->tv_sec - rhs->tv_sec + r / CARROT_NSECS_IN_SEC,
    .tv_nsec = r % CARROT_NSECS_IN_SEC };
}


__nonnull((1,2))
int carrot_timespec_compare(struct carrot_timespec const *lhs, struct carrot_timespec const *rhs) {
  return lhs->tv_sec < rhs->tv_sec? -1
       : lhs->tv_sec > rhs->tv_sec? 1
       : lhs->tv_nsec < rhs->tv_nsec? -1
       : lhs->tv_nsec > rhs->tv_nsec? 1
       : 0;
}



