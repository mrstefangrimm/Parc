// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#include <SoftwareSerial.h>

#include "ParcLib.h"
#include "Src/Registers.h"
#include "Src/KeypadAo.h"
#include "Src/HidAo.h"
#include "Src/TerminalAo.h"
#include "Src/MemoryMonitorAo.h"

#include "UnoBreadboard.h"
#include "Constants.h"
#include "ProgramSteps.h"

using namespace unoparc;

struct LoggerFake {
  void begin(uint16_t a) {}
  template<typename T>
  void print(T ch) { }
  template<typename T>
  void print(T ch, uint8_t mode) {}
  template<typename T>
  void println(T ch) {}
  template<typename T>
  void println(T ch, uint8_t mode) {}
  void println() {}
};

struct HidBleFake {
};


typedef SoftwareSerial TerminalConsole_t;
TerminalConsole_t terminalConsole(Usb_ORA, Usb_YEL);

typedef LoggerFake Logger_t;
Logger_t logger;
//#define logger terminalConsole

typedef HardwareSerial HidUsb_t;
typedef HidBleFake HidBle_t;
typedef UnoBreadboard<Logger_t> Keypad_t;

Program<Logger_t> programs[NumberOfPrograms];
RegisterData_t registers[TOTAL_REGISTERS] = { 0 };


HidBle_t hidBle;
Keypad_t keypadHw(logger);

KeypadAo<Logger_t, Keypad_t> keypad(registers, logger, keypadHw);
HidAo<Logger_t> hid(logger, registers, programs);
MemoryMonitorAo<Logger_t> memoryMonitor(logger, registers);

template<> bool CmdComparator<PsType::Wait>::equals(const char* another) const { return 'W' == another[0]; }
template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) const { return 'U' == another[0] && 'K' == another[1]; }

struct ProgramStepFake : public ProgramStep<Logger_t> {
  static const uint8_t Radix = 0;
  static const uint8_t KeyCodeDel = 0;
  static const uint8_t KeyCodeTab = 0;
  static const uint8_t KeyCodeEnter = 0;
  static const uint8_t KeyCodeSpace = 0;

  ProgramStepFake(Logger_t& logger, HidUsb_t& ble, KeyCode keyCode, uint8_t repetitions) : ProgramStep(logger, 0) {}
  ProgramStepFake(Logger_t& logger, HidUsb_t& usb, const char* text) : ProgramStep<Logger_t>(logger, 0) {}
  ProgramStepFake(Logger_t& logger, HidBle_t& ble, const char* text) : ProgramStep<Logger_t>(logger, 0) {}
  ProgramStepFake(Logger_t& logger, HidBle_t& ble, KeyCode keyCode) : ProgramStep(logger, 0) {}
  ProgramStepFake(Logger_t& logger, HidBle_t& ble, KeyCode keyCode, uint8_t repetitions) : ProgramStep(logger, 0) {}
  ProgramStepFake(Logger_t& logger, HidBle_t& ble, const char* ctrlKey, uint16_t duration) : ProgramStep(logger, duration) {}
  void action(VirtualAction type) override {};
};

// Has to filled in the order of the enum PsType, that is:
//  Wait, USB Keycode, USB Text, BLE Keycode, BLE Text, BLE Control Key
typedef Typelist<ProgramStepWait<Logger_t>,
  Typelist<ProgramStepUsbKeyboardCode<Logger_t, HidUsb_t>,
  Typelist<ProgramStepFake,
  Typelist<ProgramStepFake,
  Typelist<ProgramStepFake,
  Typelist<ProgramStepFake,
  Typelist<ProgramStepFake,
  Typelist<ProgramStepFake,
  NullType>>>>>>>> ProgramStepList;

TerminalAo<ProgramStepList, TerminalConsole_t, Logger_t, HidBle_t, HidUsb_t, 30> terminal(terminalConsole, logger, hidBle, Serial, registers, programs);

void setup() {
  for (int n=0; n<50 && !Serial; n++) { delay(100); }
  Serial.begin(9600);
  terminalConsole.begin(9600);  
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  keypadHw.pinMode<KeyPadSwitch::Btn_A>();
  keypadHw.pinMode<KeyPadSwitch::Btn_B>();
  keypadHw.pinMode<KeyPadSwitch::Btn_C>();
  keypadHw.pinMode<KeyPadSwitch::Btn_D>();
  keypadHw.pinMode<KeyPadSwitch::Btn_E>();
  keypadHw.pinMode<KeyPadSwitch::Sw_M0>();

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
