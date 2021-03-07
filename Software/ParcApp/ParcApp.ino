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

using namespace parc;
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
Logger_t logger(Usb_ORA, Usb_YEL); // #define logger Serial

Program<Logger_t> programs[NumberOfPrograms];
RegisterData_t registers[TOTAL_REGISTERS] = { 0 };

KeypadHw<Logger_t> keypadHw(logger);
KeypadAo<Logger_t, KeypadHw<Logger_t>> keypad(registers, logger, keypadHw);

typedef HidBle<Logger_t> HidBle_t;
HidBle_t hidBle(logger);

HidAo<Logger_t> hid(logger, registers, programs);
MemoryMonitorAo<Logger_t> memoryMonitor(logger, registers);

template<> bool CmdComparator<PsType::Wait>::equals(const char* another) { return 'W' == another[0]; }
template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) { return 'U' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::UsbText>::equals(const char* another) { return 'U' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleKeycode>::equals(const char* another) { return 'B' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::BleText>::equals(const char* another) { return 'B' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleControlkey>::equals(const char* another) { return 'B' == another[0] && 'C' == another[1]; }
template<> bool CmdComparator<CmdType::Pin>::equals(const char* another) { return 'P' == another[0] && 'N' == another[1]; }

// Has to filled in the order of the enum PsType, that is:
//  Wait, USB Keycode, USB Text, BLE Keycode, BLE Text, BLE Control Key
typedef Typelist<ProgramStepWait<Logger_t>,
  Typelist<ProgramStepUsbKeyboardCode<Logger_t, Keyboard_>,
  Typelist<ProgramStepUsbKeyboardText<Logger_t, Keyboard_>,
  Typelist<ProgramStepBleKeyboardCode<Logger_t, HidBle_t>,
  Typelist<ProgramStepBleKeyboardText<Logger_t, HidBle_t>,
  Typelist<ProgramStepBleControlKey<Logger_t, HidBle_t>,
  NullType>>>>>> ProgramStepList;

TerminalAo<ProgramStepList, Serial_, Logger_t, HidBle_t, Keyboard_, 25> terminal(Serial, logger, hidBle, Keyboard, registers, programs);

void setup() {
  for (int n=0; n<5 && !Serial; n++) { delay(100); }
  Serial.begin(9600);  
  logger.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  keypadHw.begin();
  keypadHw.pinMode<true, INPUT_PULLUP, Btn_A>();
  keypadHw.pinMode<true, INPUT_PULLUP, Btn_B>();
  keypadHw.pinMode<true, INPUT_PULLUP, Btn_C>();
  keypadHw.pinMode<true, INPUT_PULLUP, Btn_D>();
  keypadHw.pinMode<true, INPUT_PULLUP, Btn_E>();
  keypadHw.pinMode<true, INPUT_PULLUP, Btn_P0>();
  keypadHw.pinMode<true, INPUT_PULLUP, Btn_P1>();
  keypadHw.pinMode<false, INPUT_PULLUP, Code_0>();
  keypadHw.pinMode<false, INPUT_PULLUP, Code_1>();
  keypadHw.pinMode<false, INPUT_PULLUP, Code_2>();
  keypadHw.pinMode<false, INPUT_PULLUP, Code_3>();

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
