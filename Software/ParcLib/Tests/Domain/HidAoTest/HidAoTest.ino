// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>

#include "Domain/HidAo.h"
#include "Domain/Program.h"
#include "Domain/Shared.h"

using namespace parclib;

struct FakeLogger {
  template<class T>
  void print(T ch) { }
  template<class T>
  void print(T ch, uint8_t mode) {}
  template<class T>
  void println(T ch) {}
} logger;


// Test naming scheme: Given-When-Then

test(given_state_idle_when_hid_input_then_timeout_set) {

  FakeLogger logger;
  Program<FakeLogger> programs[NumberOfPrograms]; memset(programs, 0, sizeof(Program<FakeLogger>));
  RegisterData_t registers[TOTAL_REGISTERS]; memset(registers, 0, sizeof(RegisterData_t));
 
  HidAo<FakeLogger, Program<FakeLogger>> hid(logger, registers, programs);

  KeypadRegData hidInput(0, 1);

  registers[KEYPAD_HID_INPUT] = hidInput.raw;
  hid.checkRegisters();

  assertEqual(0, registers[KEYPAD_HID_INPUT]);
  assertEqual(1, registers[HID_HID_TIMEOUT]);
}


void setup() {
  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  Test::run();
}
