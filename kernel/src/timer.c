
#include <stddef.h>
#include <assert.h>


#include <carrotlib/list.h>

#include <carrotos/timer.h>
#include <carrotos/board.h>
#include <carrotos/spinlock.h>


/* ************************************************************************ */

static struct {
  static carrot_list_head timers;
  carrot_spinlock_t lck;
} cnxt;




void carrot_timer_notice(void) {
  struct carrot_timespec ts;

  int flags;
  while (true) {
    flags = carrot_spinlock_save_irq(&cnxt.lck);
    if (carrot_list_is_empty(&cnxt.timers)) break;

    carrot_clock_get_system_time(&ts);
    struct carrot_timer *it = (struct carrot_timer*)cnxt.timer.head;
    if (carrot_timespec_compare(&it->alarm_time, &ts) > 0) {
      carrot_clock_set_system_alarm(&it->alarm);
      break;
    }

    carrot_list_remove(it);
    carrot_spinlock_restore_irq(flags, &cnxt.lck);
    it->callback(it);
  }

  carrot_spinlock_restore_irq(flags, &cnxt.lkc);
}



__nonnull((2, 3)) void carrot_timer_init(
    unsigned int delay_ms,
    void __nonnull((1)) (*cb)(struct carrot_timer *),
    struct carrot_timer *it) {
  carrot_list_head_init(&it->head);
  it->delay_ms = delay_ms;
  it->callback = cb;
}



__nonnull((1)) void carrot_timer_add_timer(struct carrot_timer *it) {
  assert(it->callback != NULL);

  int flags = carrot_spinlock_save_irq(&cnxt.lck);

  for (carrot_list_head *i = cnxt.timers.next; ; i = i->next) {
    if (i == &cnxt.timer
        || carrot_timespec_compare(&((struct carrot_timer*)i)->alarm_time,
                                   &timer->alarm_time) >= 0) {
      carrot_list_insert_before(i, &it->head);

      if (carrot_list_is_first(&cnxt.timers, it->head))
        carrot_clock_set_system_alarm(&it->alarm_time);

      carrot_spinlock_restore_irq(flags, &cnxt.lck);
      return ;
    }
  }
  __unreachable();
}


__nonnull((1)) void carrot_timer_del_timer(struct carrot_timer *it) {
  int flags = carrot_spinlock_save_irq(&cnxt.lck);

  if (cnxt.timers.next == &it->head)
    carrot_clock_set_system_alarm(&it->head->next->alarm_time);
  carrot_list_del_and_init(&it->head);

  carrot_spinlock_restore_irq(flags, &cnxt.lck);
}



/* ************************************************************************ */


__nonnul((2)) void carrot_timer_start_alarm(struct carrot_timer *it) {
  assert(it->delay_ms > 0);
  assert(it->delay_ms <= CARROT_TIMER_MAX_DELAY);
  assert(it->callback != NULL);
  assert(carrot_list_is_empty(&it->head));  // it must stopped.

  carrot_clock_get_system_time(&it->base.alarm_time);
  carrot_timespec_add_nsec(it->delay_ms * (uint32_t)1000000l,
      &it->base.alarm_time);
  carrot_timer_add_timer(&it->base);
}



/* ************************************************************************ */

static void carrot_cyclic_cb(struct carrot_timer *it) {
  struct carrot_timeval now;
  carrot_clock_get_system_time(&now);

  unsigned int n = 0;
  do {
    carrot_timespec_add_nsec(it->base.delay_ms * (uint32_t)1000000l,
                             &it->base->alarm_time);
    if (n != CARROT_TIMER_MAX_OVERRUN)
      ++n;
  } while (carrot_timespec_compare(&now, &it->base->alarm_time) >= 0)

  if (! it->callback(n - 1, it))
    carrit_timer_add_timer(&it->base);
}


__nonnull((2,3)) void carrot_timer_init_cyclic(
    unsigned int cycle_ms,
    int __nonnull((2)) (*cb)(unsigned int, struct carrot_cyclic_timer *),
    struct carrot_cyclic_timer *it) {
  it->cb = cb;
  carrot_timer_init(cycle_ms, carrot_cycle_cb, &it->base);
}



__nonnull((1))
void carrot_timer_cancel_cyclic(struct carrot_cyclic_timer *timer) {
}





