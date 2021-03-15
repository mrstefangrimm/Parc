// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "Shared.h"

namespace parclib {

  template<class TLOGGER, class TKEYPADHW>
  class KeypadAo : public Ao<KeypadAo<TLOGGER, TKEYPADHW>> {
  public:
    KeypadAo(RegisterData_t* registers, TLOGGER& logger, TKEYPADHW& keypadHw)
      : Ao_t(registers), _log(logger), _hw(keypadHw) {}

    void checkRegisters() {

      if (Ao_t::_registers[TERMINAL_KEYPAD_PIN] != 0) {
        if (_pin.raw == 0) {
          _pin = Ao_t::_registers[TERMINAL_KEYPAD_PIN];
        }
        else {
          _log.println(F("PIN not accepted."));
          Ao_t::_registers[KEYPAD_TERMINAL_PINALREADYDEFINED] = PinAlreadyDefinedRegData(true);
        }
        Ao_t::_registers[TERMINAL_KEYPAD_PIN] = 0;
      }
      else if (Ao_t::_registers[KEYPAD_KEYPAD_TIMEOUT] != 0) {

        Ao_t::_registers[KEYPAD_KEYPAD_TIMEOUT] -= 1;
        if (Ao_t::_registers[KEYPAD_KEYPAD_TIMEOUT] == 0) {

          KeypadRegData args = 0;          

          // Buttons A - E, Multiple button presses is not supported
          if (_hw.template pressed<KeyPadSwitch::Btn_A>()) {
            args.button = A;
          }
          else if (_hw.template pressed<KeyPadSwitch::Btn_B>()) {
            args.button = B;
          }
          else if (_hw.template pressed<KeyPadSwitch::Btn_C>()) {
            args.button = C;
          }
          else if (_hw.template pressed<KeyPadSwitch::Btn_D>()) {
            args.button = D;
          }
          else if (_hw.template pressed<KeyPadSwitch::Btn_E>()) {
            args.button = E;
          }          
         
          bool longTimeout = false;
          if (args.button != 0) {

            // Read mode 0 - 3
            if (_hw.template pressed<KeyPadSwitch::Sw_M0>()) {
              args.mode = 1;
            }
            if (_hw.template pressed<KeyPadSwitch::Sw_M1>()) {
              args.mode |= (1 << 1);
            }
            // Debug: _log.println(args.mode);

            uint8_t pin = 0;
            if (_hw.template pressed<KeyPadSwitch::Code_1>()) {
              pin = 1;
            }
            if (_hw.template pressed<KeyPadSwitch::Code_2>()) {
              pin |= (1 << 1);
            }
            if (_hw.template pressed<KeyPadSwitch::Code_3>()) {
              pin |= (1 << 2);
            }
            if (_hw.template pressed<KeyPadSwitch::Code_4>()) {
              pin |= (1 << 3);
            }
            
            // Debug: _log.print(_pin.pin(), BIN); _log.print(F(" ")); _log.println(pin, BIN);

            if (_pin.raw == 0 || _pin.pin() == pin) {
              _pin.failed = 0;
              // Debug: _log.println(args.button);
              Ao_t::_registers[KEYPAD_HID_INPUT] = args.raw;
              Ao_t::_registers[KEYPAD_MEMORY_WRONG] = _pin.raw;
            }
            else if (_pin.raw != 0 && pin != 0) {
              _log.print(F("Wrong PIN, remaining retries: ")); _log.println(_pin.retries - _pin.failed);
              if (_pin.failed == _pin.retries) {
                _log.println(F("Game Over."));
                Ao_t::_registers[KEYPAD_MEMORY_WRONG] = _pin.raw;
              }
              else {
                _pin.failed++;
                longTimeout = true;
                Ao_t::_registers[KEYPAD_MEMORY_WRONG] = _pin.raw;
              }
            }
            else {
              _log.print(F("Buttion press ignored.")); _log.print(_pin.pin(), BIN); _log.print(F(" ")); _log.println(pin, BIN);
            }
          }

          if (longTimeout) {
            // Debug: _log.print(F("1"));
            Ao_t::_registers[KEYPAD_KEYPAD_TIMEOUT] = TimerRegData(5000 / TimerPeriod);
          }
          else {
            // Debug: _log.print(F("5"));
            Ao_t::_registers[KEYPAD_KEYPAD_TIMEOUT] = TimerRegData(1);
          }
        }
      }
    }   

  private:
    typedef Ao<KeypadAo<TLOGGER, TKEYPADHW>> Ao_t;
    typedef TKEYPADHW Hw_t;

    enum KeypadButton { A = 1, B, C, D, E };

    TLOGGER& _log;
    Hw_t& _hw;

    PinRegData _pin;
  };

}
