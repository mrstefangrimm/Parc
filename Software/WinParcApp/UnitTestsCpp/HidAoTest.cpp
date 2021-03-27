// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"
#include "CppUnitTest.h"

#define F(x) (x)
#define max(a,b) ((a)>(b)?(a):(b))
enum SerialFakeMode { BIN, HEX };
const int INPUT_PULLUP = 1;
const int OUTPUT = 2;

#include "Src/HidAo.h"
#include "Src/Program.h"
#include "Src/Shared.h"
#include "FakeLogger.h"

namespace UnitTestsCpp 
{

  using namespace Microsoft::VisualStudio::CppUnitTestFramework;
  using namespace parclib;


  // Test naming scheme: Given-When-Then

  TEST_CLASS(HidAoTest)
  {
  public:

    TEST_METHOD(given_state_idle_when_hid_input_then_timeout_set) {

      FakeLogger logger;
      Program<FakeLogger> programs[NumberOfPrograms]; memset(programs, 0, sizeof(Program<FakeLogger>));
      RegisterData_t registers[TOTAL_REGISTERS]; memset(registers, 0, sizeof(RegisterData_t));

      HidAo<FakeLogger, Program<FakeLogger>> hid(logger, registers, programs);

      KeypadRegData hidInput(0, 1);

      registers[KEYPAD_HID_INPUT] = hidInput.raw;
      hid.checkRegisters();

      Assert::AreEqual<uint8_t>(0, registers[KEYPAD_HID_INPUT]);
      Assert::AreEqual<uint8_t>(1, registers[HID_HID_TIMEOUT]);
    }

  };

}
