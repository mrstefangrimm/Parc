// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>
#include "ParcLib.h"
#include "Tests/NotquiteBDD.h"

#include "ProgramTest.h"

void setup() {
  while (!Serial)
    ;

  Serial.begin(9600);
}

void loop() {
  Test::run();
}
