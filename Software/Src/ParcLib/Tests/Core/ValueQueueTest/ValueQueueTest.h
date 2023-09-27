// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/ValueQueue.h"

namespace ValueQueueTest {

using namespace parclib;

BEGIN(ValueQueueTest)

TEST(
  queue_with_size_1,
  items_queued_and_dequeued_sequentially,
  pop_gets_latest_push) {

  ValueQueue<1> q;
  q.push(1);
  EQ((uint8_t)1, q.pop());
  q.push(2);
  EQ((uint8_t)2, q.pop());
}

TEST(
  queue_with_size_1,
  dequeue_on_empty_queue,
  pop_gets_zero) {

  ValueQueue<1> q;
  q.push(1);
  EQ((uint8_t)1, q.pop());
  EQ((uint8_t)0, q.pop());
  EQ((uint8_t)0, q.pop());
}

TEST(
  queue_with_size_2,
  items_queued_and_dequeued_sequentially,
  pop_gets_latest_push) {

  ValueQueue<2> q;
  q.push(1);
  EQ((uint8_t)1, q.pop());
  q.push(2);
  EQ((uint8_t)2, q.pop());
}

TEST(
  queue_with_size_2,
  dequeue_on_empty_queue,
  pop_gets_zero) {

  ValueQueue<2> q;
  q.push(1);
  EQ((uint8_t)1, q.pop());
  EQ((uint8_t)0, q.pop());
  EQ((uint8_t)0, q.pop());
}

TEST(
  queue_with_size_2,
  two_items_queued,
  pop_gets_first_than_second_item) {

  ValueQueue<2> q;
  q.push(1);
  q.push(2);
  EQ((uint8_t)1, q.pop());
  EQ((uint8_t)2, q.pop());
  EQ((uint8_t)0, q.pop());
}

TEST(
  queue_with_size_2,
  items_dequeued_while_queue,
  pop_always_returns_first_queued_item) {

  ValueQueue<2> q;

  q.push(1);

  for (int n = 2; n < 10; n++) {
    q.push(n);
    EQ((uint8_t)(n - 1), q.pop());
  }
}

TEST(
  queue_with_size_1_and_uint32_t,
  items_queued_and_dequeued_sequentially,
  pop_gets_latest_push) {

  ValueQueue<1, uint32_t> q;
  q.push(1);
  EQ((uint32_t)1, q.pop());
  q.push(2);
  EQ((uint32_t)2, q.pop());
}

END
}
