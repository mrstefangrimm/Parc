// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>
#include <StandardCplusplus.h> // Commented out all code in new_handler.cpp is commented out. This fix did not work: https://github.com/mike-matera/ArduinoSTL/issues/66 (ArduinoSTL does not create output, so I used StandardCplusplus)
#include "ParcLib.h"
#include "Tests/NotquiteBDD.h"

void assert(bool condition) { TRUE(condition); }

#include "ValueQueueTest.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  Test::run();
}
