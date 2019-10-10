

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <carrotlib/ilst.h>

#include <carrotos/spinlock.h>
#include <carrotos/board.h>


struct carrot_tcb {
  carrot_list_head_t head;

  void *stack_head;
  size_t stack_depth_bytes;

  void *stack_ptr;
  carrot_waitq_t* wq;
  int wup_reason;

  uint8_t base_prio;
  uint8_t actual_prio;
};
typedef carrot_tcb* carrot_task_t;


struct carrot_wait_queue {
  carrot_list_head_t tasks;
};


struct carrot_semaphore {
  unsigned int count;
  carrot_wait_queue wq;
  carrot_spinlock_t lck;
};


