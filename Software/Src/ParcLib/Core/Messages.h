// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "ValueQueue.h"

namespace parclib {

  using MessageData_t = uint8_t;

  struct Messages {
    ValueQueue<1, MessageData_t> fromKeypadToTerminalQueue;
    ValueQueue<1, MessageData_t> fromTerminalToKeypadQueue;
    ValueQueue<1, MessageData_t> fromKeypadToHidQueue;
    ValueQueue<1, MessageData_t> fromKeypadToServiceMonitorQueue;
    ValueQueue<1, MessageData_t> fromTerminalToServiceMonitorQueue;
  };

  using TimerRegData = MessageData_t;
  using PinAlreadyDefinedRegData = MessageData_t;
  using ProgramChangedRegData = MessageData_t;

struct KeypadRegData {
  KeypadRegData() : raw(0) {}
  explicit KeypadRegData(MessageData_t rawValue) : raw(rawValue) {}
  KeypadRegData(uint8_t mode, uint8_t button) : mode(mode), button(button), isPin(0) {}

  uint8_t programIndex() {
    return ((mode << 3) | button) - 3 * mode - 1;
  }

  union {
    struct {
      MessageData_t mode : 2;
      MessageData_t button : 3;
      MessageData_t isPin : 1;
    };
    MessageData_t raw;
  };
};

struct PinRegData {
  PinRegData() : raw(0) {}
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
