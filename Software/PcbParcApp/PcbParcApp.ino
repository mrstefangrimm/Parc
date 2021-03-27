// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#include <SoftwareSerial.h>

#include "ParcLib.h"
#include "Src/Registers.h"

#include "KeypadHw.h"
#include "Src/KeypadAo.h"

#include <Keyboard.h>
#include "HidBle.h"
#include "Src/HidAo.h"

#include "Src/TerminalAo.h"

#include "Src/MemoryMonitorAo.h"

#include "Constants.h"
#include "ProgramSteps.h"

using namespace pcbparc;
using namespace parclib;

class FakeLogger {
public:
  FakeLogger(uint8_t a, uint8_t b) {}
  void begin(uint16_t a) {}
  template<typename T>
  void print(T ch) { }
  template<typename T>
  void print(T ch, uint8_t mode) {}
  template<typename T>
  void println(T ch) {}
  template<typename T>
  void println(T ch, uint8_t mode) {}
};

typedef SoftwareSerial Logger_t; // Serial_, HardwareSerial, SoftwareSerial, FakeLogger
Logger_t logger(Usb_ORA, Usb_YEL); // typedef Serial_ Logger_t; #define logger Serial

Program<Logger_t> programs[NumberOfPrograms];
RegisterData_t registers[TOTAL_REGISTERS] = { 0 };

KeypadHw<Logger_t> keypadHw(logger);
KeypadAo<Logger_t, KeypadHw<Logger_t>> keypad(registers, logger, keypadHw);

typedef HidBle<Logger_t> HidBle_t;
HidBle_t hidBle(logger);

HidAo<Logger_t, Program<Logger_t>> hid(logger, registers, programs);
MemoryMonitorAo<Logger_t, 216> memoryMonitor(logger, registers);

template<> bool CmdComparator<PsType::Wait>::equals(const char* another) const { return 'W' == another[0]; }
template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) const { return 'U' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::UsbText>::equals(const char* another) const { return 'U' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleKeycode>::equals(const char* another) const { return 'B' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::BleText>::equals(const char* another) const { return 'B' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleControlkey>::equals(const char* another) const { return 'B' == another[0] && 'C' == another[1]; }
template<> bool CmdComparator<CmdType::Pin>::equals(char** another) const { return 'P' == another[0][0] && 'N' == another[1][0]; }

// Has to filled in the order of the enum PsType, that is:
//  Wait, USB Keycode, USB Keycode repeated, USB Keycodes, USB Text, BLE Keycode, BLE Keycode repeated, BLE Text, BLE Control Key
typedef Typelist<ProgramStepWait<Logger_t>,
  Typelist<ProgramStepUsbKeyboardCode<Logger_t, Keyboard_>,
  Typelist<ProgramStepUsbKeyboardCodeRepeated<Logger_t, Keyboard_>,
  Typelist<ProgramStepUsbKeyboardCodes<Logger_t, Keyboard_>,
  Typelist<ProgramStepUsbKeyboardText<Logger_t, Keyboard_>,
  Typelist<ProgramStepBleKeyboardCode<Logger_t, HidBle_t>,
  Typelist<ProgramStepBleKeyboardCodeRepeated<Logger_t, HidBle_t>,
  Typelist<ProgramStepBleKeyboardText<Logger_t, HidBle_t>,
  Typelist<ProgramStepBleControlKey<Logger_t, HidBle_t>,
  NullType>>>>>>>>> ProgramStepList;

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

TerminalAo<ProgramStepList, Serial_, Logger_t, HidBle_t, Keyboard_, Program<Logger_t>, KnownKeycodes, 30> terminal(Serial, logger, hidBle, Keyboard, registers, programs);

void setup() {
  for (int n=0; n<50 && !Serial; n++) { delay(100); }
  Serial.begin(9600);  
  logger.begin(9600);
  
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

  hidBle.begin(false, false);
  Keyboard.begin();

  registers[KEYPAD_KEYPAD_TIMEOUT] = TimerRegData(1);
  registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
  registers[MEMORY_MEMORY_TIMEOUT] = TimerRegData(10);
}

void loop() {

  keypad.checkRegisters();
  hid.checkRegisters();
  terminal.checkRegisters();
  memoryMonitor.checkRegisters();
  
  delay(TimerPeriod);  
}
