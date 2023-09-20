// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "Shared.h"
#include "BitTimer.h"

namespace parclib {

template<class TLOGGERFAC, class TKEYPADHW>
class KeypadAo : public Ao<KeypadAo<TLOGGERFAC, TKEYPADHW>> {
  public:
    KeypadAo(Register* registers, TKEYPADHW& keypadHw)
      : Ao_t(registers), _hw(keypadHw) {
      _timer = Timer_t((1 << 7) - 1);
    }

    void checkRegisters() {

      if (Ao_t::_registers->get(TERMINAL_KEYPAD_PIN) != 0) {
        if (_pin.raw == 0) {
          _pin = PinRegData(Ao_t::_registers->get(TERMINAL_KEYPAD_PIN));
        }
        else {
          auto log = TLOGGERFAC::create();
          log->println(F("PIN not accepted."));
          Ao_t::_registers->set(KEYPAD_TERMINAL_PINALREADYDEFINED, PinAlreadyDefinedRegData(true));
        }
        Ao_t::_registers->set(TERMINAL_KEYPAD_PIN, 0);
      }
      else if (_timer.increment()) {

        KeypadRegData args;

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

          auto log = TLOGGERFAC::create();
          // Debug: _log.print(_pin.pin(), BIN); _log.print(F(" ")); _log.println(pin, BIN);

          if (_pin.raw == 0 || _pin.pin() == pin) {
            _pin.failed = 0;
            // Debug: _log.println(args.button);
            Ao_t::_registers->set(KEYPAD_HID_INPUT, args.raw);
            Ao_t::_registers->set(KEYPAD_MONITOR_WRONGPIN, _pin.raw);
          }
          else if (_pin.raw != 0 && pin != 0) {
            log->print(F("Wrong PIN, remaining retries: ")); log->println(_pin.retries - _pin.failed);
            if (_pin.failed == _pin.retries) {
              log->println(F("Game Over."));
              _pin.setGameOver();
              Ao_t::_registers->set(KEYPAD_MONITOR_WRONGPIN, _pin.raw);
            }
            else {
              _pin.failed++;
              longTimeout = true;
              Ao_t::_registers->set(KEYPAD_MONITOR_WRONGPIN, _pin.raw);
            }
          }
          else {
            log->println(F("Button press ignored."));
            // Debug: _log.print(_pin.pin(), BIN); _log.print(F(" ")); _log.println(pin, BIN);
          }
        }

        if (longTimeout) {
          // Debug: _log.print(F("1"));
          _timer = Timer_t((1 << 7) - (5000 / TimerPeriod));
        }
        else {
          // Debug: _log.print(F("5"));
          _timer = Timer_t((1 << 7) - 1);
        }
      }
    }

  private:
    using Ao_t = Ao<KeypadAo<TLOGGERFAC, TKEYPADHW>>;
    using Hw_t = TKEYPADHW;
    using Timer_t = BitTimer<7, uint8_t>;

    enum KeypadButton { A = 1, B, C, D, E };

    Hw_t& _hw;

    PinRegData _pin;
    Timer_t _timer;
};

}
