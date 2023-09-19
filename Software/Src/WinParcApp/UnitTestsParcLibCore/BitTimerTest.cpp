// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"

namespace BitTimerTest {

#include "Core/Registers.h"

  using namespace parclib;

  BEGIN(BitTimerTest)

    TEST(
      8bit_timer_with_0_bits,
      incremented_serveral_times,
      overflows_always) {

    BitTimer<0> bt;
    EQ(true, bt.increment());
    EQ(true, bt.increment());
    EQ(true, bt.increment());
  }

  TEST(
    16bit_timer_with_0_bits,
    incremented_serveral_times,
    overflows_always) {

    BitTimer<0, uint16_t> bt;
    EQ(true, bt.increment());
    EQ(true, bt.increment());
    EQ(true, bt.increment());
  }

    TEST(
      8bit_timer_with_1_bits,
      incremented_serveral_times,
      overflows_at_2) {

    BitTimer<1> bt;
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

    BitTimer<2> bt;
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
    8bit_timer_with_8_bits,
    incremented_serveral_times,
    overflows_at_256) {

    BitTimer<8> bt;
    for (int n = 0; n < 255; n++) {
      EQ(false, bt.increment());
    }
    EQ(true, bt.increment());
  }

  TEST(
    16bit_timer_with_10_bits,
    incremented_serveral_times,
    overflows_at_1024) {

    BitTimer<10, uint16_t> bt;
    for (int n = 0; n < 1023; n++) {
      EQ(false, bt.increment());
    }
    EQ(true, bt.increment());
  }

  END
}

