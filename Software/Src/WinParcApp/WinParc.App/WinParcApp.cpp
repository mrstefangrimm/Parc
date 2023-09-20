// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include <iostream>
#include <chrono>
#include <thread>
#include <list>

#include <stdio.h>
#include <tchar.h>

#include "WinParcAPIWrapper.h"

#define F(x) (x)
#define max(a,b) ((a)>(b)?(a):(b))
enum SerialFakeMode { BIN, HEX };
const int INPUT_PULLUP = 1;
const int OUTPUT = 2;

#include "Core/Ao.h"
#include "Core/Registers.h"
#include "Core/HidAo.h"
#include "Core/KeypadAo.h"
#include "Core/StringParc.h"
#include "Core/Program.h"
#include "Core/TerminalAo.h"
#include "Core/SystemMonitorAo.h"
#include "Core/Shared.h"
#include "Feather/ProgramSteps.h"

using namespace std;
using namespace parclib;

WinParcAPIWrapper parcApi;

class ApiLogger {
public:
  ApiLogger(WinParcAPIWrapper& parcApi) : _parcApi(parcApi) { }

  void print(const char* ch) {
    _parcApi.DebugPrint(ch);
  }
  void print(char ch) {
    char str[2] = { ch, '\0' };
    _parcApi.DebugPrint(str);
  }
  void print(char ch, uint8_t mode) {
    char str[2] = { ch, '\0' };
    _parcApi.DebugPrint(str);
  }
  void print(int val) {
    char str[10];
    itoa(val, str, 10);
    _parcApi.DebugPrint(str);
  }
  void println(const char* ch) { _parcApi.DebugPrintLn(ch); }
  void println(const char* ch, SerialFakeMode mode) { _parcApi.DebugPrintLn(ch); }
  void println(int val) {
    char str[10];
    itoa(val, str, 10);
    _parcApi.DebugPrintLn(str);
  }

private:
  WinParcAPIWrapper& _parcApi;
};
ApiLogger logger(parcApi);
template<> ApiLogger& Factory<ApiLogger>::instance = logger;
typedef Factory<ApiLogger> LoggerFac_t;

class ApiSerial {
public:
  ApiSerial(WinParcAPIWrapper& parcApi) : _parcApi(parcApi) { }

  void print(const char* ch) {
    _parcApi.TerminalPrint(ch);
  }
  void print(char ch) {
    char str[2] = { ch, '\0' };
    _parcApi.TerminalPrint(str);
  }
  void print(char ch, SerialFakeMode mode) {
    char str[2] = { ch, '\0' };
    _parcApi.TerminalPrint(str);
  }
  void println(const char* ch) { _parcApi.TerminalPrintLn(ch); }
  void println(const char* ch, SerialFakeMode mode) { _parcApi.TerminalPrintLn(ch); }
  void println() { _parcApi.TerminalPrintLn("\r\n"); }
  void println(int val) {
    char str[10];
    itoa(val, str, 10);
    _parcApi.TerminalPrintLn(str);
  }
  void write(const uint8_t* buf, int bufLen) { cout << (const char*)buf; }


  bool available() { return parcApi.TerminalIsAvailable(); }
  int read() {
    if (parcApi.TerminalIsAvailable()) {
      char ch = parcApi.TerminalRead();
      if (ch != 0) {
        return ch;
      }
    }
    return -1;
  }

private:
  WinParcAPIWrapper& _parcApi;
};

class ApiKeypadHw {

public:
  ApiKeypadHw(WinParcAPIWrapper& parcApi) : _parcApi(parcApi) {}

  void begin() {
  }

  template<KeyPadSwitch SWITCH>
  bool pressed() { return pressed(Int2Type<SWITCH>()); }

private:
  bool pressed(Int2Type<KeyPadSwitch::Btn_A>) { return pressed("A"); }
  bool pressed(Int2Type<KeyPadSwitch::Btn_B>) { return pressed("B"); }
  bool pressed(Int2Type<KeyPadSwitch::Btn_C>) { return pressed("C"); }
  bool pressed(Int2Type<KeyPadSwitch::Btn_D>) { return pressed("D"); }
  bool pressed(Int2Type<KeyPadSwitch::Btn_E>) { return pressed("E"); }
  bool pressed(Int2Type<KeyPadSwitch::Sw_M0>) { return pressed("M0"); }
  bool pressed(Int2Type<KeyPadSwitch::Sw_M1>) { return pressed("M1"); }
  bool pressed(Int2Type<KeyPadSwitch::Code_1>) { return pressed("C1"); }
  bool pressed(Int2Type<KeyPadSwitch::Code_2>) { return pressed("C2"); }
  bool pressed(Int2Type<KeyPadSwitch::Code_3>) { return pressed("C3"); }
  bool pressed(Int2Type<KeyPadSwitch::Code_4>) { return pressed("C4"); }

  bool pressed(const char* pin) {
    return _parcApi.KeypadPressed(pin);
  }

  WinParcAPIWrapper& _parcApi;
};

class ApiSystemHw {
public:
  ApiSystemHw(WinParcAPIWrapper& parcApi) : _parcApi(parcApi) { }

  int freeMemory() { return 1000; }
  void warnLedOn() { _parcApi.SetWarnLed(true); }
  void warnLedOff() { _parcApi.SetWarnLed(false); }

private:
  WinParcAPIWrapper& _parcApi;
};
ApiSystemHw sysHw(parcApi);
template<> ApiSystemHw& Factory<ApiSystemHw>::instance = sysHw;
typedef Factory<ApiSystemHw> SystemHwFac_t;

Program<LoggerFac_t> programs[NumberOfPrograms];
Register registers;


ApiSerial terminal(parcApi);
ApiKeypadHw keypadHw(parcApi);

KeypadAo<LoggerFac_t, ApiKeypadHw> keypadAo(&registers, keypadHw);
HidAo<LoggerFac_t, Program<LoggerFac_t>> hidAo(&registers, programs);

SystemMonitorAo<LoggerFac_t, SystemHwFac_t, 216> systemMonitor(&registers);

template<> bool CmdComparator<PsType::Wait>::equals(const char* another) const { return 'W' == another[0]; }
template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) const { return 'U' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::UsbText>::equals(const char* another) const { return 'U' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleKeycode>::equals(const char* another) const { return 'B' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::BleText>::equals(const char* another) const { return 'B' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleControlkey>::equals(const char* another) const { return 'B' == another[0] && 'C' == another[1]; }
template<> bool CmdComparator<CmdType::Pin>::equals(char** another) const { return 'P' == another[0][0] && 'N' == another[1][0]; }

struct FakeUsbKeyboard {
  void print(const char* ch) { }
  void press(uint8_t val) { }
  void releaseAll() { }
};
FakeUsbKeyboard usb;
template<> FakeUsbKeyboard& Factory<FakeUsbKeyboard>::instance = usb;
typedef Factory<FakeUsbKeyboard> HidUsbFac_t;

struct FakeHidBle {
  void sendKeyCode(KeyCode keyCode) { }
  bool waitForOK() { return true; }
  void print(const char*) { }
  void println(const char*) { }
};
FakeHidBle ble;
template<> FakeHidBle& Factory<FakeHidBle>::instance = ble;
typedef Factory<FakeHidBle> HidBleFac_t;

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

// Has to filled in the order of the enum PsType, that is:
//  Wait, USB Keycode, USB Keycode Repeated, USB Keycodes, USB Text, BLE Keycode, BLE Keycode Repeated, BLE Text, BLE Control Key
using ProgramStepList = Typelist<ProgramStepWait<LoggerFac_t>, Typelist<ProgramStepUsbKeyboardCode<LoggerFac_t, HidUsbFac_t>, Typelist<ProgramStepUsbKeyboardCodeRepeated<LoggerFac_t, HidUsbFac_t>, Typelist<ProgramStepUsbKeyboardCodes<LoggerFac_t, HidUsbFac_t>, Typelist<ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>, Typelist<ProgramStepBleKeyboardCode<LoggerFac_t, HidBleFac_t>, Typelist<ProgramStepBleKeyboardCodeRepeated<LoggerFac_t, HidBleFac_t>, Typelist<ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>, Typelist<ProgramStepBleControlKey<LoggerFac_t, HidBleFac_t>, NullType>>>>>>>>>;

TerminalAo<
  ProgramStepList,
  ApiSerial,
  LoggerFac_t,
  HidBleFac_t,
  HidUsbFac_t,
  Program<LoggerFac_t>,
  SystemHwFac_t,
  KnownKeycodes,
  40> terminalAo(terminal, &registers, programs);

void setup() {
  parcApi.Initialize();

  registers.set(MONITOR_MONITOR_TIMEOUT, TimerRegData(10));
}

void loop() {

  keypadAo.checkRegisters();
  hidAo.checkRegisters();
  terminalAo.checkRegisters();
  systemMonitor.checkRegisters();

  //if (terminal.available()) {
  //  char ch = terminal.read();
  //  if (ch != 0) {
  //    std::cout << ch;
  //  }
  //  logger.print(ch);
  //  terminal.print(ch);
  //}

  this_thread::sleep_for(chrono::milliseconds(TimerPeriod));

}

void thread_function() {
  while (true) { loop(); }
}

int _tmain(int argc, _TCHAR* argv[])
{
  setup();
  thread t(&thread_function);  // t starts running
  t.join();                    // main thread waits for the thread t to finish
  return 0;
}
