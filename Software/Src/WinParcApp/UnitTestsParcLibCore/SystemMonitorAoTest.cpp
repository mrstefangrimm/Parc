// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"
#include "CppUnitTest.h"

#define F(x) (x)

#include "Core/Shared.h"
#include "Core/SystemMonitorAo.h"

using namespace parclib;

namespace SystemMonitorAoTest {
  struct FakeLogger {
    template<class T>
    void print(T ch) { }
    template<class T>
    void print(T ch, uint8_t mode) { }
    template<class T>
    void println(T ch) { }
  };
  struct FakeSystemHw {
    int freeMemory() { return 100; }
    void warnLedOn() { isLedOn = true; }
    void warnLedOff() { isLedOn = false; }

    bool isLedOn;
  };
}
SystemMonitorAoTest::FakeLogger logger;
template<> SystemMonitorAoTest::FakeLogger& Factory<SystemMonitorAoTest::FakeLogger>::instance = logger;
typedef Factory<SystemMonitorAoTest::FakeLogger> LoggerFac_t;

SystemMonitorAoTest::FakeSystemHw sysHw;
template<> SystemMonitorAoTest::FakeSystemHw& Factory<SystemMonitorAoTest::FakeSystemHw>::instance = sysHw;
typedef Factory<SystemMonitorAoTest::FakeSystemHw> SystemHwFac_t;

namespace SystemMonitorAoTest {

  using namespace Microsoft::VisualStudio::CppUnitTestFramework;

  // Test naming scheme: Given-When-Then

  TEST_CLASS(SystemMonitorAoTest)
  {
    TEST_METHOD_INITIALIZE(Initialize)
    {
      reset();
    }

    TEST_METHOD(given_wrongpin_when_failed_then_LED_is_on_for_5_sec)
    {
      sysHw.isLedOn = false;
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 50> systemMonitorAo(registers);

      PinRegData pinData;
      pinData.failed = 2;

      registers[KEYPAD_MONITOR_WRONGPIN] = pinData.raw;
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(true, sysHw.isLedOn);

      for (int n = 0; n < 5000 / TimerPeriod; n++) {
        systemMonitorAo.checkRegisters();
      }

      Assert::AreEqual<bool>(false, sysHw.isLedOn);
    }

    TEST_METHOD(given_wrongpin_and_low_memory_when_checkRegisters_then_LED_is_on_and_remains_on)
    {
      sysHw.isLedOn = false;
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 216> systemMonitorAo(registers);

      PinRegData pinData;
      pinData.failed = 1;
      registers[KEYPAD_MONITOR_WRONGPIN] = pinData.raw;
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(true, sysHw.isLedOn);

      for (int n = 0; n < 5000 / TimerPeriod; n++) {
        systemMonitorAo.checkRegisters();
      }

      Assert::AreEqual<bool>(true, sysHw.isLedOn);
    }

    TEST_METHOD(given_progchange_and_low_memory_when_checkRegisters_then_LED_is_on)
    {
      sysHw.isLedOn = false;
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 216> systemMonitorAo(registers);

      registers[TERMINAL_MONITOR_PROGCHANGE] = ProgramChangedRegData(1);
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(true, sysHw.isLedOn);
    }

    TEST_METHOD(given_progchange_and_free_memory_when_checkRegisters_then_LED_is_off)
    {
      sysHw.isLedOn = false;
      RegisterData_t registers[TOTAL_REGISTERS] = { 0 };
      SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 50> systemMonitorAo(registers);

      registers[TERMINAL_MONITOR_PROGCHANGE] = ProgramChangedRegData(1);
      systemMonitorAo.checkRegisters();

      Assert::AreEqual<bool>(false, sysHw.isLedOn);
    }

  private:
    void reset() const {
      SystemHwFac_t::instance.isLedOn = 0;
    }
  };

}
