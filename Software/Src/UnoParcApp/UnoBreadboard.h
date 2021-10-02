// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/Shared.h"
#include "Constants.h"

namespace unoparc {

  using namespace parclib;

  template<class TLOGGERFAC>
  class UnoBreadboard {
  public: 
    template<KeyPadSwitch SWITCH>
    void pinMode() { pinMode(Int2Type<SWITCH>()); }

    template<KeyPadSwitch SWITCH>
    bool pressed() { return pressed(Int2Type<SWITCH>()); }
    
  private:
    void pinMode(Int2Type<KeyPadSwitch::Btn_A>) { pinMode(Int2Type<true>(), Pin_A); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_B>) { pinMode(Int2Type<true>(), Pin_B); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_C>) { pinMode(Int2Type<true>(), Pin_C); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_D>) { pinMode(Int2Type<true>(), Pin_D); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_E>) { pinMode(Int2Type<true>(), Pin_E); }
    void pinMode(Int2Type<KeyPadSwitch::Sw_M0>) { pinMode(Int2Type<true>(), Pin_M0); }
  
    void pinMode(Int2Type<true>, uint8_t pin) {
      auto log = TLOGGERFAC::create();
      log->print(F("Set Arduino port: ")); log->print(pin); log->println(F(" to INPUT_PULLUP."));
      ::pinMode(pin, INPUT_PULLUP);
    }
    
    void pinMode(Int2Type<false>, uint8_t pin) {
      auto log = TLOGGERFAC::create();
      log->print(F("Ignore : ")); log->println(pin);
    }

    bool pressed(Int2Type<KeyPadSwitch::Btn_A>) { return ::digitalRead(Pin_A) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_B>) { return ::digitalRead(Pin_B) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_C>) { return ::digitalRead(Pin_C) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_D>) { return ::digitalRead(Pin_D) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Btn_E>) { return ::digitalRead(Pin_E) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Sw_M0>) { return ::digitalRead(Pin_M0) == LOW; }
    bool pressed(Int2Type<KeyPadSwitch::Sw_M1>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_1>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_2>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_3>) { return false; }
    bool pressed(Int2Type<KeyPadSwitch::Code_4>) { return false; }
   };
}
