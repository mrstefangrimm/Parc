// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Adafruit_MCP23008.h"
#include "Shared.h"

namespace parc {

  template<typename TLOGGER>
  class KeypadHw {

  public:
    KeypadHw(TLOGGER& logger) : _log(logger) {}

    void begin() {
      _mcp.begin();
    }

    template<bool EXPANDERPORT, uint8_t MODE, uint8_t PIN>
    void pinMode() {
      CTAssert<MODE == INPUT_PULLUP>();
      return pinMode(Int2Type<EXPANDERPORT>(), PIN, MODE);
    }

    bool pressed(bool expanderPort, uint8_t pin) {
      if (expanderPort) {
        return _mcp.digitalRead(pin) == LOW;
      }
      else {
        return ::digitalRead(pin) == HIGH;
      }
    }
    
    template<class EXPANDERPORT, uint8_t PIN>
    bool pressed() {
      return false; //pressed(Int2Type<EXPANDERPORT>(), pin);
    }

  private:
    void error(const __FlashStringHelper* err) {
      _log.println(err);
      while (true);
    }

    void pinMode(Int2Type<true>, uint8_t pin, uint8_t mode) {
      _log.print(F("Set MCP28003 port: ")); _log.print(pin); _log.println(F(" to INPUT HIGH."));
      _mcp.pinMode(pin, INPUT);
      _mcp.pullUp(pin, HIGH);
    }
    
    void pinMode(Int2Type<false>, uint8_t pin, uint8_t mode) {
      _log.print(F("Set Arduino port: ")); _log.print(pin); _log.println(F(" to INPUT_PULLUP."));
      ::pinMode(pin, INPUT_PULLUP);
    }

    TLOGGER& _log;
    Adafruit_MCP23008 _mcp;

  };

}
