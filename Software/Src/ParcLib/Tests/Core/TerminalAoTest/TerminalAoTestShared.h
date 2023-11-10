// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include <queue>
#include <iostream>
#include "Core/HidAo.h"
#include "Core/Shared.h"
#include "Core/Program.h"
#include "Core/KeypadAo.h"
#include "Core/TerminalAo.h"

using namespace parclib;

template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) const {
  return 'U' == another[0] && 'K' == another[1];
}
template<> bool CmdComparator<PsType::Wait>::equals(const char* another) const {
  return 'W' == another[0];
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

namespace TerminalAoTestShared {

class FakeSerial {
public:
  void setInputBuffer(const char* terminalInput) {
    //std::queue<char> empty;
    //std::swap(_buf, empty);
    TRUE(_buf.empty());
    for (size_t n = 0; n < strlen(terminalInput); n++) {
      _buf.push(terminalInput[n]);
    }
  }

  template<class T>
  void print(T) {}
  template<class T>
  void print(T, int) {}
  template<class T>
  void println(T) {}
  template<class T>
  void println(T, int) {}
  void println() {}

  bool available() {
    return !_buf.empty();
  }
  int read() {
    if (available()) {
      char ret = _buf.front();
      _buf.pop();
      return ret;
    }
    return -1;
  }

private:
  std::queue<char> _buf;
};

/*
struct FakeLogger {
  template<class T>
  void print(T x) { Serial.print(x); }
  template<class T>
  void print(T x, uint8_t y) { Serial.print(x); }
  template<class T>
  void println(T x) { Serial.print(x); }
};
*/
struct FakeLogger {
  template<class T>
  void print(T) {}
  template<class T>
  void print(T, uint8_t) {}
  template<class T>
  void println(T) {}
};


struct FakeSystemHw {
  int freeMemory() {
    return 100;
  }
  void warnLedOn() {}
  void warnLedOff() {}
};

struct FakeHidBle {
  void sendKeyCode(parclib::KeyCode) {}
  bool waitForOK() {
    return true;
  }
  void print(const char*) {}
  void println(const char*) {}
};

struct FakeKeypadHw {

  template<KeyPadSwitch SWITCH>
  void pinMode() {
  }

  template<KeyPadSwitch SWITCH>
  bool pressed() {
    return false;
  }
};

template<class TLOGGERFAC>
struct FakeProgram {

  ProgramStep<TLOGGERFAC>* appendStep(ProgramStep<TLOGGERFAC>* step) {
    if (root == nullptr) {
      root = step;
      currentStep = step;
      return root;
    } else {
      return root->appendStep(step);
    }
  }

  virtual ~FakeProgram() {
    dispose();
  }

  void dispose() {
    if (root != nullptr) {
      root->dispose();
      delete root;
      root = nullptr;
    }
  }

  bool hasSteps() const {
    return root != nullptr;
  }

  ProgramStep<TLOGGERFAC>* root = nullptr;
  ProgramStep<TLOGGERFAC>* currentStep = nullptr;
};

}

TerminalAoTestShared::FakeLogger logger;
template<> TerminalAoTestShared::FakeLogger& Factory<TerminalAoTestShared::FakeLogger>::instance = logger;
typedef Factory<TerminalAoTestShared::FakeLogger> LoggerFac_t;

TerminalAoTestShared::FakeSystemHw sysHw;
template<> TerminalAoTestShared::FakeSystemHw& Factory<TerminalAoTestShared::FakeSystemHw>::instance = sysHw;
typedef Factory<TerminalAoTestShared::FakeSystemHw> SystemHwFac_t;

TerminalAoTestShared::FakeHidBle ble;
template<> TerminalAoTestShared::FakeHidBle& Factory<TerminalAoTestShared::FakeHidBle>::instance = ble;
typedef Factory<TerminalAoTestShared::FakeHidBle> HidBleFac_t;

namespace TerminalAoTestShared {

FakeProgram<LoggerFac_t> programs[NumberOfPrograms];
parclib::Messages messages;

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

struct FakeProgramStep : public ProgramStep<LoggerFac_t> {
  explicit FakeProgramStep(uint16_t)
    : ProgramStep<LoggerFac_t>(0) {
    type = PsType::Wait;
  }
  explicit FakeProgramStep(const char*)
    : ProgramStep<LoggerFac_t>(0) {
    type = PsType::UsbText;
  }
  explicit FakeProgramStep(parclib::KeyCode keyCode)
    : ProgramStep(0) {
    type = PsType::UsbKeycode;
    hexCode = keyCode.hexCode;
  }
  FakeProgramStep(parclib::KeyCode keyCode, uint8_t)
    : ProgramStep(0) {
    type = PsType::UsbKeycodeRepeated;
    hexCode = keyCode.hexCode;
  }
  FakeProgramStep(parclib::KeyCode keyCode, char)
    : ProgramStep<LoggerFac_t>(0) {
    type = PsType::UsbKeycodes;
    hexCode = keyCode.hexCode;
  }

  void action(VirtualAction, uint8_t&) override{};

  FakeProgramStep* next() {
    return static_cast<FakeProgramStep*>(_next);
  }

  PsType type;
  uint8_t hexCode = 0;
};

struct FakeBleProgramStep : public ProgramStep<LoggerFac_t> {
  explicit FakeBleProgramStep(const char* text)
    : ProgramStep<LoggerFac_t>(0) {
    type = strcmp("Mute", text) == 0 ? type = PsType::BleControlkey : PsType::BleText;
  }
  explicit FakeBleProgramStep(parclib::KeyCode keyCode)
    : ProgramStep(0) {
    type = PsType::BleKeycode;
    hexCode = keyCode.hexCode;
  }
  FakeBleProgramStep(parclib::KeyCode keyCode, uint8_t)
    : ProgramStep(0) {
    type = PsType::BleKeycodeRepeated;
    hexCode = keyCode.hexCode;
  }

  void action(VirtualAction, uint8_t&) override{};

  PsType type;
  uint8_t hexCode = 0;
};

FakeKeypadHw keypadHw;
KeypadAo<LoggerFac_t, FakeKeypadHw> keypadAo(messages, keypadHw);
HidAo<LoggerFac_t, FakeProgram<LoggerFac_t>> hid(messages, programs);

// Has to filled in the order of the enum PsType, that is:
//  Wait, USB Keycode, USB Keycode Repeated, USB Keycodes, USB Text, BLE Keycode, BLE Keycode Repeated, BLE Text, BLE Control Key
using ProgramStepList =
  Typelist<FakeProgramStep,
           Typelist<FakeProgramStep,
                    Typelist<FakeProgramStep,
                             Typelist<FakeProgramStep,
                                      Typelist<FakeProgramStep,
                                               Typelist<FakeBleProgramStep,
                                                        Typelist<FakeBleProgramStep,
                                                                 Typelist<FakeBleProgramStep,
                                                                          Typelist<FakeBleProgramStep, parclib::NullType>>>>>>>>>;

FakeSerial serial;
using TerminalAo_t = TerminalAo<ProgramStepList, FakeSerial, LoggerFac_t, HidBleFac_t, parclib::NullType, FakeProgram<LoggerFac_t>, SystemHwFac_t, KnownKeycodes, 40>;

struct TestTerminalAo : TerminalAo_t {

  enum State {
    Init,
    Idle,
    ReadingProgramCode,
    ReadingProgramSteps,
    ReadingPin
  };

  tsmlib::DispatchResult<Sm::StatePolicy> lastResult = tsmlib::DispatchResult<Sm::StatePolicy>::null;

  TestTerminalAo(FakeSerial& serialInput, parclib::Messages& messages, FakeProgram<LoggerFac_t>* programs)
    : TerminalAo_t(serialInput, messages, programs) {
  }

  // Careful: This hides the TerminalAo implementation
  void run() {
    if (_timer.current()) {
      lastResult = _statemachine.dispatch(Trigger::Timeout{ this });

      if (_pinDefinedMsg != 0) {
        _serial.println(F("PIN was not accepted. A PIN is already active."));
      }

      _pinDefinedMsg = 0;
    }
  }

  TestTerminalAo::State getState() {

    if (lastResult.activeState == nullptr) return Init;

    auto someidle = Idle::CreatorType::create();
    if (someidle == lastResult.activeState) return Idle;

    auto someReadingProgramCode = ReadingProgramCode::CreatorType::create();
    if (someReadingProgramCode == lastResult.activeState) return ReadingProgramCode;

    auto someReadingProgramSteps = ReadingProgramSteps::CreatorType::create();
    if (someReadingProgramSteps == lastResult.activeState) return ReadingProgramSteps;

    auto someReadingPin = ReadingPin::CreatorType::create();
    if (someReadingPin == lastResult.activeState) return ReadingPin;
  }
};

TestTerminalAo terminal(serial, messages, programs);
}