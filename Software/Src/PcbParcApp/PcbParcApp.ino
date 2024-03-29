// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#include <SoftwareSerial.h>
#include <Keyboard.h>
#include <TimerOne.h>

#include "ParcLib.h"
#include "Core/Messages.h"
#include "Core/KeypadAo.h"
#include "Core/HidAo.h"
#include "Core/TerminalAo.h"
#include "Core/SystemMonitorAo.h"

#include "Feather/HidBle.h"
#include "Feather/ProgramSteps.h"
#include "Feather/SystemHw.h"

#include "KeypadHw.h"
#include "Constants.h"

using namespace parclib;
using namespace pcbparc;

void rtassert(bool condition) {
}

struct FakeLogger {
  FakeLogger(uint8_t a, uint8_t b) {}
  void begin(uint16_t a) {}
  template<typename T>
  void print(T ch) {}
  template<typename T>
  void print(T ch, uint8_t mode) {}
  template<typename T>
  void println(T ch) {}
  template<typename T>
  void println(T ch, uint8_t mode) {}
};
typedef FakeLogger Logger_t;  // SoftwareSerial, FakeLogger
Logger_t logger(Usb_ORA, Usb_YEL);
template<> Logger_t& Factory<Logger_t>::instance = logger;
typedef Factory<Logger_t> LoggerFac_t;

template<> Keyboard_& Factory<Keyboard_>::instance = Keyboard;
typedef Factory<Keyboard_> HidUsbFac_t;

typedef HidBle<LoggerFac_t> HidBle_t;
HidBle_t ble;
template<> HidBle_t& Factory<HidBle_t>::instance = ble;
typedef Factory<HidBle_t> HidBleFac_t;

SystemHw sysHw;
template<> SystemHw& Factory<SystemHw>::instance = sysHw;
typedef Factory<SystemHw> SystemHwFac_t;

Program<LoggerFac_t> programs[NumberOfPrograms];
Messages messages;

KeypadHw<LoggerFac_t> keypadHw;
KeypadAo<LoggerFac_t, KeypadHw<LoggerFac_t>> keypad(messages, keypadHw);

HidAo<LoggerFac_t, Program<LoggerFac_t>> hid(messages, programs);

SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 216> systemMonitor(messages);

template<> bool CmdComparator<PsType::Wait>::equals(const char* another) const {
  return 'W' == another[0];
}
template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) const {
  return 'U' == another[0] && 'K' == another[1];
}
template<> bool CmdComparator<PsType::UsbText>::equals(const char* another) const {
  return 'U' == another[0] && 'T' == another[1];
}
template<> bool CmdComparator<PsType::BleKeycode>::equals(const char* another) const {
  return 'B' == another[0] && 'K' == another[1];
}
template<> bool CmdComparator<PsType::BleText>::equals(const char* another) const {
  return 'B' == another[0] && 'T' == another[1];
}
template<> bool CmdComparator<PsType::BleControlkey>::equals(const char* another) const {
  return 'B' == another[0] && 'C' == another[1];
}
template<> bool CmdComparator<CmdType::Pin>::equals(char** another) const {
  return 'P' == another[0][0] && 'N' == another[1][0];
}

// Has to filled in the order of the enum PsType, that is:
//  Wait, USB Keycode, USB Keycode repeated, USB Keycodes, USB Text, BLE Keycode, BLE Keycode repeated, BLE Text, BLE Control Key
typedef Typelist<ProgramStepWait<LoggerFac_t>,
                 Typelist<ProgramStepUsbKeyboardCode<LoggerFac_t, HidUsbFac_t>,
                          Typelist<ProgramStepUsbKeyboardCodeRepeated<LoggerFac_t, HidUsbFac_t>,
                                   Typelist<ProgramStepUsbKeyboardCodes<LoggerFac_t, HidUsbFac_t>,
                                            Typelist<ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>,
                                                     Typelist<ProgramStepBleKeyboardCode<LoggerFac_t, HidBleFac_t>,
                                                              Typelist<ProgramStepBleKeyboardCodeRepeated<LoggerFac_t, HidBleFac_t>,
                                                                       Typelist<ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>,
                                                                                Typelist<ProgramStepBleControlKey<LoggerFac_t, HidBleFac_t>,
                                                                                         NullType>>>>>>>>>
  ProgramStepList;

struct KnownKeycodes {
  static const uint8_t UsbRadix = 16;
  static const uint8_t UsbKeyCodeDel = 0xD4;
  static const uint8_t UsbKeyCodeTab = 0xB3;
  static const uint8_t UsbKeyCodeEnter = 0xB0;
  static const uint8_t UsbKeyCodeSpace = ' ';

  static const uint8_t BleRadix = 16;
  static const uint8_t BleKeyCodeDel = 0x4C;
  static const uint8_t BleKeyCodeTab = 0x2B;
  static const uint8_t BleKeyCodeEnter = 0x28;
  static const uint8_t BleKeyCodeSpace = 0x2C;
};

TerminalAo<ProgramStepList, Serial_, LoggerFac_t, HidBleFac_t, HidUsbFac_t, Program<LoggerFac_t>, SystemHwFac_t, KnownKeycodes, 30> terminal(Serial, messages, programs);

volatile bool tick_flag = false;
volatile bool fatalError = false;

void setup() {
  while (!Serial)
    ;

  Serial.begin(9600);
  logger.begin(9600);
  Keyboard.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  keypadHw.begin();
  keypadHw.pinMode<KeyPadSwitch::Btn_A>();
  keypadHw.pinMode<KeyPadSwitch::Btn_B>();
  keypadHw.pinMode<KeyPadSwitch::Btn_C>();
  keypadHw.pinMode<KeyPadSwitch::Btn_D>();
  keypadHw.pinMode<KeyPadSwitch::Btn_E>();
  keypadHw.pinMode<KeyPadSwitch::Sw_M0>();
  keypadHw.pinMode<KeyPadSwitch::Sw_M1>();
  keypadHw.pinMode<KeyPadSwitch::Code_1>();
  keypadHw.pinMode<KeyPadSwitch::Code_2>();
  keypadHw.pinMode<KeyPadSwitch::Code_3>();
  keypadHw.pinMode<KeyPadSwitch::Code_4>();

  ble.begin(false, false);
  Keyboard.begin();

  Timer1.initialize(1000L * TimerPeriod);
  Timer1.attachInterrupt(timer1InterruptHandler);
}

void loop() {
  rtassert(!fatalError);
  if (tick_flag) {
    keypad.load();
    hid.load();
    terminal.load();
    systemMonitor.load();

    keypad.run();
    hid.run();
    terminal.run();
    systemMonitor.run();

    tick_flag = false;
  }
}

void timer1InterruptHandler() {
  if (tick_flag == true) fatalError = true;
  tick_flag = true;
}
