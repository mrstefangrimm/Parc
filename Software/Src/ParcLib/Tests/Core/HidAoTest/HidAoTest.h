// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/HidAo.h"
#include "Core/Program.h"
#include "Core/Shared.h"

namespace HidAoTest {

struct FakeLogger {
  template<class T>
  void print(T ch) {}
  template<class T>
  void print(T ch, uint8_t mode) {}
  template<class T>
  void println(T ch) {}
};
}

HidAoTest::FakeLogger logger;
template<> HidAoTest::FakeLogger& parclib::Factory<HidAoTest::FakeLogger>::instance = logger;
using LoggerFac_t = parclib::Factory<HidAoTest::FakeLogger>;

namespace HidAoTest {

using namespace parclib;

struct FakeProgramStep : public ProgramStep<LoggerFac_t> {
  FakeProgramStep()
    : ProgramStep<LoggerFac_t>(10), isPlaying(false), currentTick(0) {}

  void action(VirtualAction type, uint8_t& tick) override {
    isPlaying = type == VirtualAction::Tick;
    currentTick = tick;
  };

  bool isPlaying;
  uint8_t currentTick;
};

// Test naming scheme: Given-When-Then

BEGIN(HidAoTest)

TEST(
  state_idle_and_no_program,
  checkRegisters,
  timeout_not_set_as_state_idle) {

  Program<LoggerFac_t> programs[NumberOfPrograms];
  memset(programs, 0, NumberOfPrograms * sizeof(Program<LoggerFac_t>));
  RegisterData_t registers[TOTAL_REGISTERS];
  memset(registers, 0, TOTAL_REGISTERS * sizeof(RegisterData_t));

  HidAo<LoggerFac_t, Program<LoggerFac_t>> hid(registers, programs);

  KeypadRegData hidInput(0, 1);

  registers[KEYPAD_HID_INPUT] = hidInput.raw;
  hid.checkRegisters();

  EQ((uint8_t)0, registers[KEYPAD_HID_INPUT]);
  EQ((uint8_t)0, registers[HID_HID_TIMEOUT]);
}

TEST(
  state_idle_and_program,
  checkRegisters,
  state_execute) {

  Program<LoggerFac_t> programs[NumberOfPrograms];
  memset(programs, 0, NumberOfPrograms * sizeof(Program<LoggerFac_t>));
  RegisterData_t registers[TOTAL_REGISTERS];
  memset(registers, 0, TOTAL_REGISTERS * sizeof(RegisterData_t));

  HidAo<LoggerFac_t, Program<LoggerFac_t>> hid(registers, programs);

  KeypadRegData hidInput(0, 1);
  FakeProgramStep someProgramStep;
  programs[0].appendStep(&someProgramStep);

  registers[KEYPAD_HID_INPUT] = hidInput.raw;

  hid.checkRegisters();
  // Changed from state Idle to State Execute
  EQ((uint8_t)0, registers[KEYPAD_HID_INPUT]);
  EQ((uint8_t)1, registers[HID_HID_TIMEOUT]);
  EQ((bool)false, someProgramStep.isPlaying);
  EQ((uint8_t)0, someProgramStep.currentTick);

  hid.checkRegisters();
  // State Execute
  EQ((uint8_t)0, registers[KEYPAD_HID_INPUT]);
  EQ((uint8_t)1, registers[HID_HID_TIMEOUT]);
  EQ((bool)true, someProgramStep.isPlaying);
  EQ((uint8_t)0, someProgramStep.currentTick);
}

END

}