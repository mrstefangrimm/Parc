// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Adafruit_MCP23008.h"

namespace parc {

  template<typename TLOGGER>
  class KeypadHw {

  public:
    KeypadHw(TLOGGER& logger) : _log(logger) {}

    void begin() {
      _mcp.begin();
    }

    void pinMode(uint8_t port, uint8_t mode, bool expander) {
      if (mode != INPUT_PULLUP) {
        error(F("Pin mode not supported"));
      }
      if (expander) {
        _log.print(F("Set MCP28003 port: ")); _log.print(port); _log.println(F(" to INPUT HIGH."));
        _mcp.pinMode(port, INPUT);
        _mcp.pullUp(port, HIGH);
      }
      else {
        _log.print(F("Set Arduino port: ")); _log.print(port); _log.println(F(" to INPUT_PULLUP."));
        ::pinMode(port, INPUT_PULLUP);
      }
    }

    bool pressed(uint8_t port, bool expander) {
      if (expander) {
        return _mcp.digitalRead(port) == LOW;
      }
      else {
        if (port == 9) { return false; }
        return ::digitalRead(port) == HIGH;
      }
    }

  private:
    void error(const __FlashStringHelper* err) {
      _log.println(err);
      while (true);
    }
    TLOGGER& _log;
    Adafruit_MCP23008 _mcp;

  };

}
