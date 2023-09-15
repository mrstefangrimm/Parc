// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>
#include "ParcLib.h"

#define BEGIN(a)
#define END
#define TEST(given, when, then) test(GIVEN_##given##_WHEN_##when##_THEN_##then)
#define EQ(a, b) assertEqual(a, b)

#include "HidAoTest.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  Test::run();
}
