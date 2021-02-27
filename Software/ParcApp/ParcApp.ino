// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#include <SoftwareSerial.h>

#include "Registers.h"

#include "KeypadHw.h"
#include "KeypadAo.h"

#include <Keyboard.h>
#include "HidBle.h"
#include "HidAo.h"

#include "TerminalAo.h"

#include "MemoryMonitorAo.h"

#include "Constants.h"

using namespace parc;

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
typedef Keyboard_ HidUsb_t;
HidBle_t hidBle(logger);

HidAo<Logger_t> hid(logger, registers, programs);
MemoryMonitorAo<Logger_t> memoryMonitor(logger, registers);

template<> bool CmdComparator<PsType::Wait>::equals(const char* another) { return 'W' == another[0]; }
template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) { return 'U' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::UsbText>::equals(const char* another) { return 'U' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleKeycode>::equals(const char* another) { return 'B' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::BleText>::equals(const char* another) { return 'B' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleControlkey>::equals(const char* another) { return 'B' == another[0] && 'C' == another[1]; }

// Has to filled in the same order as the enum PsType
typedef Typelist<ProgramStepWait<Logger_t>,
  Typelist<ProgramStepUsbKeyboardCode<Logger_t, HidUsb_t>,
  Typelist<ProgramStepUsbKeyboardText<Logger_t, HidUsb_t>,
  Typelist<ProgramStepBleKeyboardCode<Logger_t, HidBle_t>,
  Typelist<ProgramStepBleKeyboardText<Logger_t, HidBle_t>,
  Typelist<ProgramStepBleControlKey<Logger_t, HidBle_t>,
  NullType>>>>>> ProgramStepList;

TerminalAo<ProgramStepList, Serial_, Logger_t, HidBle_t, HidUsb_t, 25> terminal(Serial, logger, hidBle, Keyboard, registers, programs);

void setup() {
  for (int n=0; n<5 && !Serial; n++) { delay(100); }
  Serial.begin(9600);  
  logger.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  keypadHw.begin();
  keypadHw.pinMode(Btn_A, INPUT_PULLUP, true);
  keypadHw.pinMode(Btn_B, INPUT_PULLUP, true);
  keypadHw.pinMode(Btn_C, INPUT_PULLUP, true);
  keypadHw.pinMode(Btn_D, INPUT_PULLUP, true);
  keypadHw.pinMode(Btn_E, INPUT_PULLUP, true);
  keypadHw.pinMode(Btn_P0, INPUT_PULLUP, true);
  keypadHw.pinMode(Btn_P1, INPUT_PULLUP, true);
  keypadHw.pinMode(Code_0, INPUT_PULLUP, false);
  keypadHw.pinMode(Code_1, INPUT_PULLUP, false);
  keypadHw.pinMode(Code_2, INPUT_PULLUP, false);
  keypadHw.pinMode(Code_3, INPUT_PULLUP, false);

  hidBle.begin(false, false);
  Keyboard.begin();

  registers[KEYPAD_KEYPAD_TIMEOUT] = TimerRegData(1);
  registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
  registers[MEMORY_MEMORY_TIMEOUT] = TimerRegData(10);

  logger.println(sizeof(ProgramStep<Logger_t>));  
  logger.println(sizeof(ProgramStepWait<Logger_t>));
  logger.println(sizeof(ProgramStepWait<Logger_t>));
  logger.println(sizeof(ProgramStepBleKeyboardCode<Logger_t, HidBle_t>));
  
}

void loop() {

  keypad.checkRegisters();
  hid.checkRegisters();
  terminal.checkRegisters();
  memoryMonitor.checkRegisters();
  
  delay(TimerPeriod);  
}
