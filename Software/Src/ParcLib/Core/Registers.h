// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parclib {

  enum Events {
    KEYPAD_KEYPAD_TIMEOUT = 1,           // 6 bit (0-50)
    TERMINAL_KEYPAD_PIN,                 // 8 bit (PinRegData)
    KEYPAD_HID_INPUT,                    // 8 bit (KeypadRegData)
    MONITOR_MONITOR_TIMEOUT,             // 8 bit (0 - 255)
    KEYPAD_MONITOR_WRONGPIN,             // 8 bit (PinRegData)
    KEYPAD_TERMINAL_PINALREADYDEFINED,   // 1 bit
    TERMINAL_TERMINAL_TIMEOUT,           // 1 bit
    TERMINAL_MONITOR_PROGCHANGE,         // 1 bit
    TOTAL_REGISTERS
  };

  // TODO: this typedef could be application specific
  typedef uint8_t RegisterData_t;

  // Doublebuffered register
  class Register {
  public:
    uint8_t get(uint8_t pos) {
      return _registers[pos];
    }

    uint8_t set(uint8_t pos, uint8_t val) {
      uint8_t currentval = _registers[pos];

      _registers[pos] = val;

      return currentval;
    }

    void store() {

    }
 
  private:
    uint8_t _registers[TOTAL_REGISTERS] = { 0 };

  };



typedef RegisterData_t TimerRegData;
typedef RegisterData_t PinAlreadyDefinedRegData;
typedef RegisterData_t ProgramChangedRegData;

template<size_t Timerbits, class Alloc = uint8_t>
struct BitTimer {

  enum { N = Timerbits };

  BitTimer() : raw(0) {}

  bool increment() {
    return ++value == 0;
  }

  union {
    struct {
      Alloc value : N;
    };
    Alloc raw;
  };
};

template<class Alloc>
struct BitTimer<0, Alloc> {

  BitTimer() {}

  bool increment() {
    return true;
  }
};

struct KeypadRegData {
  KeypadRegData() : raw(0) {}
  KeypadRegData(const KeypadRegData& t) : raw(t.raw) {}
  explicit KeypadRegData(RegisterData_t rawValue) : raw(rawValue) {}
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
  explicit PinRegData(uint8_t rawValue) : raw(rawValue) {}

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
