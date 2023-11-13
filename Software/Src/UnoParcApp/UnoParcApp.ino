// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#include <SoftwareSerial.h>
#include <TimerOne.h>

#include "ParcLib.h"
#include "Core/Messages.h"
#include "Core/KeypadAo.h"
#include "Core/HidAo.h"
#include "Core/TerminalAo.h"
#include "Core/SystemMonitorAo.h"

#include "Feather/SystemHw.h"

#include "UnoBreadboard.h"
#include "Constants.h"
#include "ProgramSteps.h"

using namespace unoparc;

void rtassert(bool condition) {
}

typedef SoftwareSerial TerminalConsole_t;
TerminalConsole_t terminalConsole(Usb_ORA, Usb_YEL);

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
  void println() {}
};
FakeLogger logger(Usb_ORA, Usb_YEL);
template<> FakeLogger& Factory<FakeLogger>::instance = logger;
typedef Factory<FakeLogger> LoggerFac_t;  // TerminalConsole_t, FakeLogger

template<> HardwareSerial& Factory<HardwareSerial>::instance = Serial;
typedef Factory<HardwareSerial> HidUsbFac_t;

NullType ble;
template<> NullType& Factory<NullType>::instance = ble;
typedef Factory<NullType> HidBleFac_t;

SystemHw sysHw;
template<> SystemHw& Factory<SystemHw>::instance = sysHw;
typedef Factory<SystemHw> SystemHwFac_t;

typedef UnoBreadboard<LoggerFac_t> Keypad_t;
Keypad_t keypadHw;

Program<LoggerFac_t> programs[NumberOfPrograms];
Messages messages;

KeypadAo<LoggerFac_t, Keypad_t> keypad(messages, keypadHw);
HidAo<LoggerFac_t, Program<LoggerFac_t>> hid(messages, programs);

SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 216> systemMonitor(messages);

template<> bool CmdComparator<PsType::Wait>::equals(const char* another) const {
  return 'W' == another[0];
}
template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) const {
  return 'U' == another[0] && 'K' == another[1];
}

struct ProgramStepFake : public ProgramStep<LoggerFac_t> {
  ProgramStepFake(KeyCode keyCode)
    : ProgramStep(0) {}
  ProgramStepFake(KeyCode keyCode, uint8_t repetitions)
    : ProgramStep(0) {}
  ProgramStepFake(const char* text)
    : ProgramStep<LoggerFac_t>(0) {}
  ProgramStepFake(KeyCode keyCode, char secondKey)
    : ProgramStep<LoggerFac_t>(0) {}
  void action(VirtualAction type, uint8_t& tick) override{};
};

// Has to filled in the order of the enum PsType, that is:
//  Wait, USB Keycode, USB Keycode repeated, USB Keycodes, USB Text, BLE Keycode, BLE Keycode repeated, BLE Text, BLE Control Key
typedef Typelist<ProgramStepWait<LoggerFac_t>,
                 Typelist<ProgramStepUsbKeyboardCode<LoggerFac_t, HidUsbFac_t>,
                          Typelist<ProgramStepFake,
                                   Typelist<ProgramStepFake,
                                            Typelist<ProgramStepFake,
                                                     Typelist<ProgramStepFake,
                                                              Typelist<ProgramStepFake,
                                                                       Typelist<ProgramStepFake,
                                                                                Typelist<ProgramStepFake,
                                                                                         NullType>>>>>>>>>
  ProgramStepList;

struct KnownKeycodes {
  static const uint8_t UsbRadix = 10;
  static const uint8_t UsbKeyCodeDel = 76;    // 0x4C
  static const uint8_t UsbKeyCodeTab = 43;    // 0x2B
  static const uint8_t UsbKeyCodeEnter = 40;  // 0x28
  static const uint8_t UsbKeyCodeSpace = 44;  // 0x2C

  static const uint8_t BleRadix = 0;
  static const uint8_t BleKeyCodeDel = 0;
  static const uint8_t BleKeyCodeTab = 0;
  static const uint8_t BleKeyCodeEnter = 0;
  static const uint8_t BleKeyCodeSpace = 0;
};

TerminalAo<ProgramStepList, TerminalConsole_t, LoggerFac_t, HidBleFac_t, HidUsbFac_t, Program<LoggerFac_t>, SystemHwFac_t, KnownKeycodes, 30> terminal(terminalConsole, messages, programs);

volatile bool tick_flag = false;
volatile bool fatalError = false;

void setup() {
  while (!Serial)
    ;

  Serial.begin(9600);
  terminalConsole.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  keypadHw.pinMode<KeyPadSwitch::Btn_A>();
  keypadHw.pinMode<KeyPadSwitch::Btn_B>();
  keypadHw.pinMode<KeyPadSwitch::Btn_C>();
  keypadHw.pinMode<KeyPadSwitch::Btn_D>();
  keypadHw.pinMode<KeyPadSwitch::Btn_E>();
  keypadHw.pinMode<KeyPadSwitch::Sw_M0>();

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