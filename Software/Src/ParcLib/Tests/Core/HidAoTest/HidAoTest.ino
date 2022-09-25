// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>

#include "Core/HidAo.h"
#include "Core/Program.h"
#include "Core/Shared.h"

using namespace parclib;

struct FakeLogger {
  template<class T>
  void print(T ch) { }
  template<class T>
  void print(T ch, uint8_t mode) { }
  template<class T>
  void println(T ch) { }
};
FakeLogger logger;
template<> FakeLogger& Factory<FakeLogger>::instance = logger;
typedef Factory<FakeLogger> LoggerFac_t;

struct FakeProgramStep : public ProgramStep<LoggerFac_t> {
  FakeProgramStep() : ProgramStep<LoggerFac_t>(10), isPlaying(false), currentTick(0) { }

  void action(VirtualAction type, uint8_t& tick) override {
    isPlaying = type == VirtualAction::Tick;
    currentTick = tick;
  };
  bool isPlaying;
  uint8_t currentTick;
};

// Test naming scheme: Given-When-Then

test(given_state_idle_and_no_program_when_checkRegisters_then_timeout_not_set_as_state_idle) {

  Program<LoggerFac_t> programs[NumberOfPrograms]; memset(programs, 0, NumberOfPrograms * sizeof(Program<LoggerFac_t>));
  RegisterData_t registers[TOTAL_REGISTERS]; memset(registers, 0, TOTAL_REGISTERS * sizeof(RegisterData_t));

  HidAo<LoggerFac_t, Program<LoggerFac_t>> hid(registers, programs);

  KeypadRegData hidInput(0, 1);

  registers[KEYPAD_HID_INPUT] = hidInput.raw;
  hid.checkRegisters();

  assertEqual(0, registers[KEYPAD_HID_INPUT]);
  assertEqual(0, registers[HID_HID_TIMEOUT]);
}

test(given_state_idle_and_program_when_checkRegisters_then_state_execute) {

  Program<LoggerFac_t> programs[NumberOfPrograms]; memset(programs, 0, NumberOfPrograms * sizeof(Program<LoggerFac_t>));
  RegisterData_t registers[TOTAL_REGISTERS]; memset(registers, 0, TOTAL_REGISTERS * sizeof(RegisterData_t));

  HidAo<LoggerFac_t, Program<LoggerFac_t>> hid(registers, programs);

  KeypadRegData hidInput(0, 1);
  FakeProgramStep someProgramStep;
  programs[0].appendStep(&someProgramStep);

  registers[KEYPAD_HID_INPUT] = hidInput.raw;

  hid.checkRegisters();
  // Changed from state Idle to State Execute
  assertEqual(0, registers[KEYPAD_HID_INPUT]);
  assertEqual(1, registers[HID_HID_TIMEOUT]);
  assertEqual(false, someProgramStep.isPlaying);
  assertEqual(0, someProgramStep.currentTick);

  hid.checkRegisters();
  // State Execute
  assertEqual(0, registers[KEYPAD_HID_INPUT]);
  assertEqual(1, registers[HID_HID_TIMEOUT]);
  assertEqual(true, someProgramStep.isPlaying);
  assertEqual(0, someProgramStep.currentTick);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  Test::run();
}
