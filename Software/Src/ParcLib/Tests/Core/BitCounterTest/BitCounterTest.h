// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/BitCounter.h"

namespace BitCounterTest {

using namespace parclib;

BEGIN(BitCounterTest)

TEST(
  8bit_timer_with_0_bits,
  incremented_serveral_times,
  overflows_always) {

  BitCounter<0> bt;
  EQ(true, bt.increment());
  EQ(true, bt.increment());
  EQ(true, bt.increment());
}

TEST(
  8bit_timer_with_0_bits,
  incremented_serveral_times,
  peek_is_always_true) {

  BitCounter<0> bt;
  EQ(true, bt.increment());
  EQ(true, bt.current());
  EQ(true, bt.increment());
  EQ(true, bt.current());
}

TEST(
  16bit_timer_with_0_bits,
  incremented_serveral_times,
  overflows_always) {

  BitCounter<0, uint16_t> bt;
  EQ(true, bt.increment());
  EQ(true, bt.increment());
  EQ(true, bt.increment());
}

TEST(
  8bit_timer_with_1_bits,
  incremented_serveral_times,
  overflows_at_2) {

  BitCounter<1> bt;
  EQ(false, bt.increment());
  EQ(true, bt.increment());
  EQ(false, bt.increment());
  EQ(true, bt.increment());
  EQ(false, bt.increment());
}

TEST(
  8bit_timer_with_2_bits,
  incremented_serveral_times,
  overflows_at_4) {

  BitCounter<2> bt;
  EQ(false, bt.increment());
  EQ(false, bt.increment());
  EQ(false, bt.increment());
  EQ(true, bt.increment());
  EQ(false, bt.increment());
  EQ(false, bt.increment());
  EQ(false, bt.increment());
  EQ(true, bt.increment());
}

TEST(
  8bit_timer_with_2_bits,
  incremented_serveral_times,
  overflows_at_4_and_peek_is_true_if_next_increment_returns_true) {

  BitCounter<2> bt;
  EQ(false, bt.increment());
  EQ(false, bt.current());
  EQ(false, bt.increment());
  EQ(false, bt.current());
  EQ(false, bt.increment());
  EQ(false, bt.current());
  EQ(true, bt.increment());
  EQ(true, bt.current());
  EQ(false, bt.increment());
  EQ(false, bt.current());
  EQ(false, bt.increment());
  EQ(false, bt.current());
  EQ(false, bt.increment());
  EQ(false, bt.current());
  EQ(true, bt.increment());
  EQ(true, bt.current());
}

TEST(
  8bit_timer_with_8_bits,
  incremented_serveral_times,
  overflows_at_256) {

  BitCounter<8> bt;
  for (int n = 0; n < 255; n++) {
    EQ(false, bt.increment());
  }
  EQ(true, bt.increment());
}

TEST(
  16bit_timer_with_10_bits,
  incremented_serveral_times,
  overflows_at_1024) {

  BitCounter<10, uint16_t> bt;
  for (int n = 0; n < 1023; n++) {
    EQ(false, bt.increment());
  }
  EQ(true, bt.increment());
}

TEST(
  16bit_timer_with_10_bits,
  initialize_value_changes,
  overflows_at_1024) {

  BitCounter<10, uint16_t> bt(2);
  EQ(false, bt.increment());
  EQ(true, bt.increment());

  bt.value = (1 << 10) - 4;
  EQ(false, bt.increment());
  EQ(false, bt.increment());
  EQ(false, bt.increment());
  EQ(true, bt.increment());

  bt = BitCounter<10, uint16_t>(2);
  EQ(false, bt.increment());
  EQ(true, bt.increment());
}

END
}
