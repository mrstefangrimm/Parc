// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/Shared.h"
#include "Core/SystemMonitorAo.h"

using namespace parclib;

namespace SystemMonitorAoTest {
struct FakeLogger {
  template<class T>
  void print(T ch) {}
  template<class T>
  void print(T ch, uint8_t mode) {}
  template<class T>
  void println(T ch) {}
};
struct FakeSystemHw {
  int freeMemory() {
    return 100;
  }
  void warnLedOn() {
    isLedOn = true;
  }
  void warnLedOff() {
    isLedOn = false;
  }

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

BEGIN(SystemMonitorAoTest)

INIT(Initialize, { reset(); })

TEST(
  wrongpin,
  failed,
  LED_is_on_for_5_sec) {
  sysHw.isLedOn = false;
  Register registers;
  SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 50> systemMonitorAo(&registers);

  PinRegData pinData;
  pinData.failed = 2;

  registers.set(KEYPAD_MONITOR_WRONGPIN, pinData.raw);
  systemMonitorAo.checkRegisters();

  EQ(true, (bool)sysHw.isLedOn);

  for (int n = 0; n < 5000 / TimerPeriod; n++) {
    systemMonitorAo.checkRegisters();
  }

  EQ(false, (bool)sysHw.isLedOn);
}

TEST(
  wrongpin_and_low_memory,
  checkRegisters,
  LED_is_on_and_remains_on) {
  sysHw.isLedOn = false;
  Register registers;
  SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 216> systemMonitorAo(&registers);

  PinRegData pinData;
  pinData.failed = 1;
  registers.set(KEYPAD_MONITOR_WRONGPIN, pinData.raw);
  systemMonitorAo.checkRegisters();

  EQ(true, (bool)sysHw.isLedOn);

  for (int n = 0; n < 5000 / TimerPeriod; n++) {
    systemMonitorAo.checkRegisters();
  }

  EQ(true, sysHw.isLedOn);
}

TEST(
  progchange_and_low_memory,
  checkRegisters,
  LED_is_on) {
  sysHw.isLedOn = false;
  Register registers;
  SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 216> systemMonitorAo(&registers);

  registers.set(TERMINAL_MONITOR_PROGCHANGE, ProgramChangedRegData(1));
  systemMonitorAo.checkRegisters();

  EQ(true, sysHw.isLedOn);
}

TEST(
  progchange_and_free_memory,
  checkRegisters,
  LED_is_off) {
  sysHw.isLedOn = false;
  Register registers;
  SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 50> systemMonitorAo(&registers);

  registers.set(TERMINAL_MONITOR_PROGCHANGE, ProgramChangedRegData(1));
  systemMonitorAo.checkRegisters();

  EQ(false, sysHw.isLedOn);
}

void reset() {
  SystemHwFac_t::instance.isLedOn = 0;
}

END
}
