// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "BitTimer.h"
#include "ProgramStep.h"
#include "Program.h"

namespace parclib {

template<class TLOGGERFAC, class TPROGRAM>
class HidAo : public Ao<HidAo<TLOGGERFAC, TPROGRAM>> {

  public:
    HidAo(Register* registers, TPROGRAM* programs)
      : Ao_t(registers), _programs(programs) {}

    TPROGRAM* programs() {
      return _programs;
    }

    void checkRegisters() {
      switch (_state) {
        case State::Idle: stateIdle(); break;
        case State::Execute: stateExecute(); break;
      };
      Ao_t::_registers->set(KEYPAD_HID_INPUT, 0);
    }

  private:
    void stateIdle() {
      if (_state == State::Idle &&  Ao_t::_registers->get(KEYPAD_HID_INPUT) != 0) {

        KeypadRegData args(Ao_t::_registers->get(KEYPAD_HID_INPUT));
        uint8_t progIdx = args.programIndex();
        _program = &_programs[progIdx];
        _ticksRemaining = _program->duration();

        auto log = TLOGGERFAC::create();
        log->print(F("HidAo:stateIdle() Received Mode: ")); log->print(args.mode); log->print(F(" Button: ")); log->print(args.button);
        log->print(F(" Prog. ")); log->print(progIdx); log->print(F(" Duration: ")); log->println(_ticksRemaining);

        // State Execute if a program is defined. `_ticksRemaining > 0` is equivalent to `_program.hasSteps()`
        if (_ticksRemaining > 0) {
          log->print(F("Execute { "));
          _state = State::Execute;
        }
      }
    }

    void stateExecute() {
      if (_state == State::Execute && _executionTimer.increment()) {

        _ticksRemaining--;
        _program->play();

        if (_ticksRemaining == 0) {
          auto log = TLOGGERFAC::create();
          log->println(F("}"));
          _program->rewind();
          _program = 0;
          _state = State::Idle;
        }
      }
    }

  private:
    typedef Ao<HidAo<TLOGGERFAC, TPROGRAM>> Ao_t;

    TPROGRAM* _programs;

    enum class State {
      Idle,
      Execute
    };
    State _state = State::Idle;
    TPROGRAM* _program = 0;
    size_t _ticksRemaining = 0;
    BitTimer<0> _executionTimer;
};

}
