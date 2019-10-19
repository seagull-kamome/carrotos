/**
 * @file
 * @brief Enable/Disable interrupts.
 * @section LICENSE
 *
 * MIT License
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
#include <stdbool.h>
#include "./compiler.h"

/* TODO: threadsafe, reentrant, multiprocessor
 * TODO: shorhand
 */

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct carrot_list_head {
  struct carrot_list_head *next;
  struct carrot_list_head *prev;
} carrot_list_head_t;


#define CARROT_LIST_INITIALIZER(name)  { .next = &(name), .prev = &(name) }

static __forceinline void carrot_list_init(carrot_list_head_t *it) {
  it->next = it;
  it->prev = it;
}

static __forceinline void carrot_list_insert(
    carrot_list_head_t *prev,
    carrot_list_head_t *next,
    carrot_list_head_t *it) {
  it->prev = prev;
  it->next = next;
  next->prev = it;
  prev->next = it;
}


static __forceinline void carrot_list_connect(
    carrot_list_head_t *prev,
    carrot_list_head_t *next) {
  next->prev = prev;
  prev->next = next;
}


static __forceinline void carrot_list_add(
    carrot_list_head_t *whence, carrot_list_head_t *it) {
  carrot_list_insert(whence, whence->next, it); }
static __forceinline void carrot_list_add_tail(
    carrot_list_head_t *whence, carrot_list_head_t *it) {
  carrot_list_insert(whence->prev, whence, it); }

static __forceinline void carrot_list_remove(carrot_list_head_t *it) {
  carrot_list_connect(it->prev, it->next); }
static __forceinline void carrot_list_remove_and_init(carrot_list_head_t *it) {
  carrot_list_remove(it); carrot_list_init(it); }

static __forceinline void carrot_list_move(
    carrot_list_head_t* whence, carrot_list_head_t *it) {
  carrot_list_remove(it); carrot_list_add(whence, it); }
static __forceinline void carrot_list_move_to_tail(
    carrot_list_head_t* head, carrot_list_head_t *it) {
  carrot_list_remove(it); carrot_list_add_tail(head, it); }


__nodiscard
static __forceinline bool carrot_list_empty(carrot_list_head_t const *it) {
  return it->next == it; }

__nodiscard
static __forceinline bool carrot_list_singleton(carrot_list_head_t const *it) {
  return !carrot_list_empty(it) && it->next == it->prev; }

__nodiscard
static __forceinline bool carrot_list_is_first(
    carrot_list_head_t const *head, carrot_list_head_t const *it) {
  return head->next == it; }

__nodiscard
static __forceinline bool carrot_list_is_tail(
    carrot_list_head_t const *head, carrot_list_head_t const *it) {
  return head->prev == it; }

static __forceinline void carrot_list_rotate(carrot_list_head_t *it) {
  if (!carrot_list_empty(it)) carrot_list_move_to_tail(it->next, it); }

#define CARROT_LIST_FOR_EACH(typ, iter, head) \
    for (carrot_list_head_t iter = (head)->next; iter != (head); iter = iter->next)

#if defined(__cplusplus)
}
#endif
// vim: ts=8 tw=80 sw=2 expandtab cindent :

