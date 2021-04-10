// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"
#include "CppUnitTest.h"

#define F(x) (x)

#include "Domain/HidAo.h"
#include "Domain/Program.h"
#include "Domain/Shared.h"

namespace HidAoTest
{
  using namespace Microsoft::VisualStudio::CppUnitTestFramework;
  using namespace parclib;

  struct FakeLogger {
    template<class T>
    void print(T ch) { }
    template<class T>
    void print(T ch, uint8_t mode) {}
    template<class T>
    void println(T ch) {}
  } logger;

  struct FakeProgramStep : public ProgramStep<FakeLogger> {
    FakeProgramStep(FakeLogger& logger) : ProgramStep<FakeLogger>(logger, 10), isPlaying(false), currentTick(0) { }

    void action(VirtualAction type, uint8_t& tick) override {
      isPlaying = type == VirtualAction::Tick;
      currentTick = tick;
    };

    bool isPlaying;
    uint8_t currentTick;
  };

  // Test naming scheme: Given-When-Then

  TEST_CLASS(HidAoTest)
  {
  public:

    TEST_METHOD(given_state_idle_and_no_program_when_checkRegisters_then_timeout_not_set_as_state_idle) {

      Program<FakeLogger> programs[NumberOfPrograms]; memset(programs, 0, NumberOfPrograms * sizeof(Program<FakeLogger>));
      RegisterData_t registers[TOTAL_REGISTERS]; memset(registers, 0, TOTAL_REGISTERS * sizeof(RegisterData_t));

      HidAo<FakeLogger, Program<FakeLogger>> hid(logger, registers, programs);

      KeypadRegData hidInput(0, 1);

      registers[KEYPAD_HID_INPUT] = hidInput.raw;
      hid.checkRegisters();

      Assert::AreEqual<uint8_t>(0, registers[KEYPAD_HID_INPUT]);
      Assert::AreEqual<uint8_t>(0, registers[HID_HID_TIMEOUT]);
    }

    TEST_METHOD(given_state_idle_and_program_when_checkRegisters_then_state_execute) {

      Program<FakeLogger> programs[NumberOfPrograms]; memset(programs, 0, NumberOfPrograms * sizeof(Program<FakeLogger>));
      RegisterData_t registers[TOTAL_REGISTERS]; memset(registers, 0, TOTAL_REGISTERS * sizeof(RegisterData_t));

      HidAo<FakeLogger, Program<FakeLogger>> hid(logger, registers, programs);

      KeypadRegData hidInput(0, 1);
      FakeProgramStep someProgramStep(logger);
      programs[0].appendStep(&someProgramStep);

      registers[KEYPAD_HID_INPUT] = hidInput.raw;

      hid.checkRegisters();
      // Changed from state Idle to State Execute
      Assert::AreEqual<uint8_t>(0, registers[KEYPAD_HID_INPUT]);
      Assert::AreEqual<uint8_t>(1, registers[HID_HID_TIMEOUT]);
      Assert::AreEqual<bool>(false, someProgramStep.isPlaying);
      Assert::AreEqual<uint8_t>(0, someProgramStep.currentTick);

      hid.checkRegisters();
      // State Execute
      Assert::AreEqual<uint8_t>(0, registers[KEYPAD_HID_INPUT]);
      Assert::AreEqual<uint8_t>(1, registers[HID_HID_TIMEOUT]);
      Assert::AreEqual<bool>(true, someProgramStep.isPlaying);
      Assert::AreEqual<uint8_t>(0, someProgramStep.currentTick);
    }

  };

}
