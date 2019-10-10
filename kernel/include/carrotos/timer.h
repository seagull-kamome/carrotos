
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <carrotlib/list.h>
#include <carrotlib/time.h>

/* ************************************************************************ */

struct carrot_timer {
  carrot_list_head_t head;
  struct carrot_timespec alarm_time;

  unsigned int delay_ms;
  void (*callback)(struct carrot_timer *timer);
};

#define CARROT_TIMER_MAX_DELAY  ((uint32_t)(-1) / (uint32_t)1000000l)

void carrot_timer_notice(void);

__nonnull((2, 3)) void carrot_timer_init(
    unsigned int delay_ms,
    void __nonnull((1)) (*cb)(struct carrot_timer *),
    struct carrot_timer *it);
__nonnull((1)) void carrot_timer_add_timer(struct carrot_timer *it);
__nonnull((1)) void carrot_timer_del_timer(struct carrot_timer *it);


/* ************************************************************************ */

__nonnull((1)) void carrot_timer_start_alarm(struct carrot_timer *it);
__nonnull((1)) static __forceinline void carrot_timer_cancel_alarm(
    struct carrot_timer *it) { carrot_timer_del_timer(it); }


/* ************************************************************************ */

struct carrot_cyclic_timer {
  struct carrot_timer base;
  int (*callback)(unsigned int, struct carrot_cyclic_timer *);
};

#define CARROT_TIMER_MAX_OVERRUN   ((unsigned int)-1)

__nonnull((2,3)) void carrot_timer_init_cyclic(
    unsigned int cycle_ms,
    int __nonnull((2)) (*cb)(unsigned int, struct carrot_cyclic_timer *),
    struct carrot_cyclic_timer *it);

__nonnull((1)) static __forceinline void carrot_timer_start_cyclic(
    struct carrot_cyclic_timer *it) { carrot_timer_start_alarm(&it->base); }

__nonnull((1)) void carrot_timer_cancel_cyclic(
    struct carrot_cyclic_timer *it);




