// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parc {
  
  enum Register {
    KEYPAD_KEYPAD_TIMEOUT = 1,
    KEYPAD_HID_INPUT,
    KEYPAD_MEMORY_WRONG,
    KEYPAD_TERMINAL_PINALREADYDEFINED,
    HID_HID_TIMEOUT,
    TERMINAL_TERMINAL_TIMEOUT,
    TERMINAL_MEMORY_CHANGE,
    TERMINAL_KEYPAD_PIN,
    MEMORY_MEMORY_TIMEOUT,
    TOTAL_REGISTERS
  };

  // TODO: this typedef could be application specific
  typedef uint8_t RegisterData_t;
  typedef RegisterData_t TimerRegData;
  typedef RegisterData_t MemoryChangedRegData;
  typedef RegisterData_t PinAlreadyDefinedRegData;

  struct KeypadRegData {
    KeypadRegData() : raw(0) {}
    KeypadRegData(const KeypadRegData& t) : raw(t.raw) {}
    KeypadRegData(RegisterData_t rawValue) : raw(rawValue) {}

    uint8_t programIndex() {
      return ((profile << 3) | button) - 3 * profile - 1;
    }

    union {
      struct {
        RegisterData_t profile : 2;
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
