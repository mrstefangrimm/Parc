// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Domain/Shared.h"

namespace unoparc {

  using namespace parclib;

  template<class TLOGGER>
  class UnoBreadboard {

  public:
    UnoBreadboard(TLOGGER& logger) : _log(logger) {}

    template<KeyPadSwitch SWITCH>
    void pinMode() { pinMode(Int2Type<SWITCH>()); }

    template<KeyPadSwitch SWITCH>
    bool pressed() { return pressed(Int2Type<SWITCH>()); }
    
  private:
    void pinMode(Int2Type<KeyPadSwitch::Btn_A>) { ::pinMode(Btn_A, INPUT_PULLUP); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_B>) { ::pinMode(Btn_B, INPUT_PULLUP); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_C>) { ::pinMode(Btn_C, INPUT_PULLUP); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_D>) { ::pinMode(Btn_D, INPUT_PULLUP); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_E>) { ::pinMode(Btn_E, INPUT_PULLUP); }
    void pinMode(Int2Type<KeyPadSwitch::Sw_M0>) { ::pinMode(Sw_M0, INPUT_PULLUP); }
  
    void pinMode(Int2Type<true>, uint8_t pin, uint8_t mode) {
      _log.print(F("Set Arduino port: ")); _log.print(pin); _log.println(F(" to INPUT_PULLUP."));
      ::pinMode(pin, INPUT_PULLUP);
    }
    
    void pinMode(Int2Type<false>, uint8_t pin, uint8_t mode) {
      _log.print(F("Ignore : ")); _log.println(pin);
    }

    bool pressed(Int2Type<KeyPadSwitch::Btn_A>) { return ::digitalRead(Btn_A) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_B>) { return ::digitalRead(Btn_B) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_C>) { return ::digitalRead(Btn_C) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_D>) { return ::digitalRead(Btn_D) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_E>) { return ::digitalRead(Btn_E) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Sw_M0>) { return ::digitalRead(Sw_M0) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Sw_M1>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_1>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_2>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_3>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_4>) { return false; }
 
  private:
    TLOGGER& _log;

  };

}
