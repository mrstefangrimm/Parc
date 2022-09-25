// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parclib {

enum Register {
  KEYPAD_KEYPAD_TIMEOUT = 1,
  TERMINAL_KEYPAD_PIN,
  HID_HID_TIMEOUT,
  KEYPAD_HID_INPUT,
  KEYPAD_TERMINAL_PINALREADYDEFINED,
  TERMINAL_TERMINAL_TIMEOUT,
  MONITOR_MONITOR_TIMEOUT,
  TERMINAL_MONITOR_PROGCHANGE,
  KEYPAD_MONITOR_WRONGPIN,
  X_MONITOR_X,
  TOTAL_REGISTERS
};

// TODO: this typedef could be application specific
typedef uint8_t RegisterData_t;
typedef RegisterData_t TimerRegData;
typedef RegisterData_t PinAlreadyDefinedRegData;
typedef RegisterData_t ProgramChangedRegData;

struct KeypadRegData {
  KeypadRegData() : raw(0) {}
  KeypadRegData(const KeypadRegData& t) : raw(t.raw) {}
  KeypadRegData(RegisterData_t rawValue) : raw(rawValue) {}
  KeypadRegData(uint8_t mode, uint8_t button) : mode(mode), button(button), isPin(0) {}

  uint8_t programIndex() {
    return ((mode << 3) | button) - 3 * mode - 1;
  }

  union {
    struct {
      RegisterData_t mode : 2;
      RegisterData_t button : 3;
      RegisterData_t isPin : 1;
    };
    RegisterData_t raw;
  };
};

struct PinRegData {
  PinRegData() : raw(0) {}
  PinRegData(const PinRegData& t) : raw(t.raw) {}
  PinRegData(uint8_t rawValue) : raw(rawValue) {}

  uint8_t pin() {
    return (code3 << 3) | (code2 << 2) | (code1 << 1) | code0;
  }

  bool isGameOver() {
    return raw == 0xFF;
  }
  void setGameOver() {
    raw = 0xFF;
  }

  union {
    struct {
      uint8_t code0 : 1;
      uint8_t code1 : 1;
      uint8_t code2 : 1;
      uint8_t code3 : 1;
      uint8_t retries : 2;
      uint8_t failed : 2;
    };
    uint8_t raw;
  };
};

}
