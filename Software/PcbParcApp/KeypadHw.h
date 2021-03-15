// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Adafruit_MCP23008.h"
#include "Src/Shared.h"
#include "Constants.h"

namespace pcbparc {

  using namespace parclib;

  template<typename TLOGGER>
  class KeypadHw {

  public:
    KeypadHw(TLOGGER& logger) : _log(logger), _mcpGPIO(0xFF) {}

    void begin() {
      _mcp.begin();
      _mcp.pinMode(5, OUTPUT);
    }
    
    template<KeyPadSwitch SWITCH>
    void pinMode() { pinMode(Int2Type<SWITCH>()); }

    template<KeyPadSwitch SWITCH>
    bool pressed() { return pressed(Int2Type<SWITCH>()); }
   
  private:
    void error(const __FlashStringHelper* err) {
      _log.println(err);
      while (true);
    }

    void pinMode(Int2Type<KeyPadSwitch::Btn_A>) { pinMode(Int2Type<false>(), Pin_A); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_B>) { pinMode(Int2Type<false>(), Pin_B); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_C>) { pinMode(Int2Type<false>(), Pin_C); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_D>) { pinMode(Int2Type<false>(), Pin_D); }
    void pinMode(Int2Type<KeyPadSwitch::Btn_E>) { pinMode(Int2Type<true>(), Pin_E); }
    void pinMode(Int2Type<KeyPadSwitch::Sw_M0>) { pinMode(Int2Type<true>(), Pin_M0); }
    void pinMode(Int2Type<KeyPadSwitch::Sw_M1>) { pinMode(Int2Type<true>(), Pin_M1); }
    void pinMode(Int2Type<KeyPadSwitch::Code_1>) { pinMode(Int2Type<true>(), Pin_C1); }
    void pinMode(Int2Type<KeyPadSwitch::Code_2>) { pinMode(Int2Type<true>(), Pin_C2); }
    void pinMode(Int2Type<KeyPadSwitch::Code_3>) { pinMode(Int2Type<true>(), Pin_C3); }
    void pinMode(Int2Type<KeyPadSwitch::Code_4>) { pinMode(Int2Type<true>(), Pin_C4); }
    
    void pinMode(Int2Type<true>, uint8_t pin) {
      _log.print(F("Set MCP28003 port: ")); _log.print(pin); _log.println(F(" to INPUT HIGH."));
      _mcp.pinMode(pin, INPUT);
      _mcp.pullUp(pin, HIGH);
    }
    
    void pinMode(Int2Type<false>, uint8_t pin) {
      _log.print(F("Set Arduino port: ")); _log.print(pin); _log.println(F(" to INPUT_PULLUP."));
      ::pinMode(pin, INPUT_PULLUP);
    }

    bool pressed(Int2Type<KeyPadSwitch::Btn_A>) { return pressed(Int2Type<false>(), Pin_A); }
    bool pressed(Int2Type<KeyPadSwitch::Btn_B>) { return pressed(Int2Type<false>(), Pin_B); }
    bool pressed(Int2Type<KeyPadSwitch::Btn_C>) { return pressed(Int2Type<false>(), Pin_C); }
    bool pressed(Int2Type<KeyPadSwitch::Btn_D>) { return pressed(Int2Type<false>(), Pin_D); }
    bool pressed(Int2Type<KeyPadSwitch::Btn_E>) { return pressed(Int2Type<true>(), Pin_E); }
    bool pressed(Int2Type<KeyPadSwitch::Sw_M0>) { return pressed(Int2Type<true>(), Pin_M0); }
    bool pressed(Int2Type<KeyPadSwitch::Sw_M1>) { return pressed(Int2Type<true>(), Pin_M1); }
    bool pressed(Int2Type<KeyPadSwitch::Code_1>) { return pressed(Int2Type<true>(), Pin_C1); }
    bool pressed(Int2Type<KeyPadSwitch::Code_2>) { return pressed(Int2Type<true>(), Pin_C2); }
    bool pressed(Int2Type<KeyPadSwitch::Code_3>) { return pressed(Int2Type<true>(), Pin_C3); }
    bool pressed(Int2Type<KeyPadSwitch::Code_4>) { return pressed(Int2Type<true>(), Pin_C4); }
    
    bool pressed(Int2Type<true>, uint8_t pin) {
      // Button E is read every 100 ms. The others can use _mcpGPIO.
      if (pin == Pin_E) {        
        uint8_t tmp = _mcp.readGPIO();
        // 0xFF means nothing is pressed.
        if (tmp == 0xFF) { return false; }
        // _mcpGPIO is only set if either Btn E is pressed or Modes and Codes are set. tmp is sometimes wrongly 0xFF.
        _mcpGPIO = tmp;        
      }
      // Debug: else _log.println(_mcpGPIO, BIN); // // see Constants.h, M1-M0-unused-C4-C3-C2-C1-BtnE 

      // Same as Adafruit_MCP23008.cpp - digitalRead()
      return !((_mcpGPIO >> pin) & 0x1);
    }
    
    bool pressed(Int2Type<false>, uint8_t pin) {
      return ::digitalRead(pin) == LOW;
    }

    TLOGGER& _log;
    Adafruit_MCP23008 _mcp;
    uint8_t _mcpGPIO;
  };

}
