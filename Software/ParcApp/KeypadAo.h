// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "Shared.h"

namespace parc {

  // external constants (avoids multiple usage)
  extern const uint8_t Btn_A;
  extern const uint8_t Btn_B;
  extern const uint8_t Btn_C;
  extern const uint8_t Btn_D;
  extern const uint8_t Btn_E;
  extern const uint8_t Btn_P0;
  extern const uint8_t Btn_P1;
  extern const uint8_t Code_0;
  extern const uint8_t Code_1;
  extern const uint8_t Code_2;
  extern const uint8_t Code_3;
  
  enum KeypadProfile {
    P1 = 0, P2, P3, P4
  };

  enum KeypadButton {
    A = 1, B, C, D, E
  };

  template<typename TLOGGER, typename TKEYPADHW>
  class KeypadAo : public Ao<KeypadAo<TLOGGER, TKEYPADHW>> {
  public:
    KeypadAo(RegisterData_t* registers, TLOGGER& logger, TKEYPADHW& keypadHw)
      : Ao<KeypadAo<TLOGGER, TKEYPADHW>>(registers), _log(logger), _hw(keypadHw) {}

    void checkRegisters() {

      if (Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[TERMINAL_KEYPAD_PIN] != 0) {
        if (_pin.raw == 0) {
          _pin = Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[TERMINAL_KEYPAD_PIN];
        }
        else {
          _log.println(F("PIN not accepted."));
          Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_TERMINAL_PINALREADYDEFINED] = PinAlreadyDefinedRegData(true);
        }
        Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[TERMINAL_KEYPAD_PIN] = 0;
      }
      else if (Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_KEYPAD_TIMEOUT] != 0) {

        Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_KEYPAD_TIMEOUT] -= 1;
        if (Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_KEYPAD_TIMEOUT] == 0) {

          KeypadRegData args = 0;

          // Profiles 1 - 4
          if (_hw.pressed(Btn_P0, true)) {
            args.profile = 1;
          }

          if (_hw.pressed(Btn_P1, true)) {
            args.profile |= (1 << 1);
          }

          // Buttons A - E, Multiple button presses is not supported
          if (_hw.pressed(Btn_A, true)) {
            args.button = A;
          }
          else if (_hw.pressed(Btn_B, true)) {
            args.button = B;
          }
          else if (_hw.pressed(Btn_C, true)) {
            args.button = C;
          }
          else if (_hw.pressed(Btn_D, true)) {
            args.button = D;
          }
          else if (_hw.pressed(Btn_E, true)) {
            args.button = E;
          }

          uint8_t pin = 0;
          if (_hw.pressed(Code_0, false)) {
            pin = 1;
          }
          if (_hw.pressed(Code_1, false)) {
            pin |= (1 << 1);
          }
          if (_hw.pressed(Code_2, false)) {
            pin |= (1 << 2);
          }
          if (_hw.pressed(Code_3, false)) {
            pin |= (1 << 3);
          }

          // Debug: _log.print(_pin.pin(), BIN); _log.print(F(" ")); _log.println(pin, BIN);          
          // Debug: if (args.profile != 0) { _log.println(args.profile); }
          // Debug: if (args.code != 0) { _log.println(args.code, BIN); }
          bool longTimeout = false;
          if (args.button != 0) {

            if (_pin.raw == 0 || _pin.pin() == pin) {
              _pin.failed = 0;
              // Debug: _log.println(args.button);
              Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_HID_INPUT] = args.raw;
              Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_MEMORY_WRONG] = _pin.raw;
            }
            else if (_pin.raw != 0 && pin != 0) {
              _log.print(F("Wrong PIN, remaining retries: ")); _log.println(_pin.retries - _pin.failed);
              if (_pin.failed == _pin.retries) {
                _log.println(F("Game Over."));
                Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_MEMORY_WRONG] = _pin.raw;
              }
              else {
                _pin.failed++;
                longTimeout = true;
                Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_MEMORY_WRONG] = _pin.raw;
              }
            }
            else {
              _log.print(F("Buttion press ignored.")); _log.print(_pin.pin(), BIN); _log.print(F(" ")); _log.println(pin, BIN);
            }
          }

          if (longTimeout) {
            // Debug: _log.print(F("1"));
            Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_KEYPAD_TIMEOUT] = TimerRegData(5000 / TimerPeriod);
          }
          else {
            // Debug: _log.print(F("5"));
            Ao<KeypadAo<TLOGGER, TKEYPADHW>>::_registers[KEYPAD_KEYPAD_TIMEOUT] = TimerRegData(1);
          }
        }
      }
    }

  private:
    TLOGGER& _log;
    TKEYPADHW& _hw;

    PinRegData _pin;
  };

}
