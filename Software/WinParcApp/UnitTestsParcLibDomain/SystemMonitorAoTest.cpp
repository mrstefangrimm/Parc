// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"
#include "CppUnitTest.h"

#define F(x) (x)

#include "Domain/Shared.h"
#include "Domain/SystemMonitorAo.h"

namespace SystemMonitorAoTest {

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

  struct FakeSystemHw {
    static size_t freeMemory() { return 100; }
    static void warnLedOn() { isLedOn = true; }
    static void warnLedOff() { isLedOn = false; }

    static bool isLedOn;
  };
  bool FakeSystemHw::isLedOn = false;

  // Test naming scheme: Given-When-Then

  TEST_CLASS(SystemMonitorAoTest)
  {
  public:

    TEST_METHOD(given_wrongpin_when_failed_then_LED_is_on_for_5_sec)
    {
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<FakeLogger, FakeSystemHw, 50> systemMonitorAo(logger, registers);

      PinRegData pinData;
      pinData.failed = 2;

      registers[KEYPAD_MONITOR_WRONGPIN] = pinData.raw;
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(true, FakeSystemHw::isLedOn);

      for (int n = 0; n < 5000 / TimerPeriod; n++) {
        systemMonitorAo.checkRegisters();
      }

      Assert::AreEqual<bool>(false, FakeSystemHw::isLedOn);
    }

    TEST_METHOD(given_wrongpin_and_low_memory_when_checkRegisters_then_LED_is_on_and_remains_on)
    {
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<FakeLogger, FakeSystemHw, 216> systemMonitorAo(logger, registers);

      PinRegData pinData;
      pinData.failed = 1;
      registers[KEYPAD_MONITOR_WRONGPIN] = pinData.raw;
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(true, FakeSystemHw::isLedOn);

      for (int n = 0; n < 5000 / TimerPeriod; n++) {
        systemMonitorAo.checkRegisters();
      }

      Assert::AreEqual<bool>(true, FakeSystemHw::isLedOn);
    }

    TEST_METHOD(given_progchange_and_low_memory_when_checkRegisters_then_LED_is_on)
    {
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<FakeLogger, FakeSystemHw, 216> systemMonitorAo(logger, registers);

      registers[TERMINAL_MONITOR_PROGCHANGE] = ProgramChangedRegData(1);
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(true, FakeSystemHw::isLedOn);
    }

    TEST_METHOD(given_progchange_and_free_memory_when_checkRegisters_then_LED_is_off)
    {
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<FakeLogger, FakeSystemHw, 50> systemMonitorAo(logger, registers);

      registers[TERMINAL_MONITOR_PROGCHANGE] = ProgramChangedRegData(1);
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(false, FakeSystemHw::isLedOn);
    }

  };

}
