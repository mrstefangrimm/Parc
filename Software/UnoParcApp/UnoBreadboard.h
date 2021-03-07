// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Src/Shared.h"

namespace unoparc {

  using namespace parclib;

  template<typename TLOGGER>
  class UnoBreadboard {

  public:
    UnoBreadboard(TLOGGER& logger) : _log(logger) {}

    template<uint8_t MODE, uint8_t PIN>
    void pinMode() {
      CTAssert<MODE == INPUT_PULLUP>();
      return pinMode(Int2Type<PIN != 0>(), PIN, MODE);
    }

    bool pressed(bool expanderPort, uint8_t pin) {
      if (pin == 0) { return false; }
      return ::digitalRead(pin) == LOW;
    }
    
  private:
    void pinMode(Int2Type<true>, uint8_t pin, uint8_t mode) {
      _log.print(F("Set Arduino port: ")); _log.print(pin); _log.println(F(" to INPUT_PULLUP."));
      ::pinMode(pin, INPUT_PULLUP);
    }
    
    void pinMode(Int2Type<false>, uint8_t pin, uint8_t mode) {
      _log.print(F("Ignore : ")); _log.println(pin);
    }
  
    TLOGGER& _log;

  };

}
