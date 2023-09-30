// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>
#include "ParcLib.h"
#include "Tests/NotquiteBDD.h"

void rtassert(bool condition) {
  TRUE(condition);
}

#include "HidAoTest.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  Test::run();
}
