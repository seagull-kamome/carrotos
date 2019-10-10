
#pragma once
#include <carrotlib/compiler.h>
#include <stdbool.h>
#include <stdatomic.h>

typedef uint32_t carrot_atomic_t;
static __forceinline bool carrot_atomic_compare_and_exchange_strong(
    carrot_atomic_t volatile *object,
    carrot_atomic_t *expected,
    carrot_atomic_t desired) {
  if (*object == *expected) {
    *object = desired;
    return true;
  } else {
    *expected = *object;
    return false;
  }
}

