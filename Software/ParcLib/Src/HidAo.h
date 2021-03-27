// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "ProgramStep.h"
#include "Program.h"

namespace parclib {
  
  template<class TLOGGER, class TPROGRAM>
  class HidAo : public Ao<HidAo<TLOGGER, TPROGRAM>> {

  public:
    HidAo(TLOGGER& logger, RegisterData_t* registers, TPROGRAM* programs)
      : Ao_t(registers), _log(logger), _programs(programs) {}
    
    TPROGRAM* programs() { return _programs; }

    void checkRegisters() {
      switch (_state) {
        case State::Idle: stateIdle(); break;
        case State::Execute: stateExecute(); break;
      };
      Ao_t::_registers[KEYPAD_HID_INPUT] = 0;
    }
    
  private:
    void stateIdle() {
      if (_state == State::Idle &&  Ao_t::_registers[KEYPAD_HID_INPUT] != 0) {

        KeypadRegData args(Ao_t::_registers[KEYPAD_HID_INPUT]);
        uint8_t progIdx = args.programIndex();
        _program = &_programs[progIdx];
        _ticksRemaining = _program->duration();

        _log.print(F("HidAo:stateIdle() Received Mode: ")); _log.print(args.mode); _log.print(F(" Button: ")); _log.print(args.button);
        _log.print(F(" Prog. ")); _log.print(progIdx); _log.print(F(" Duration: ")); _log.println(_ticksRemaining);

        _log.print(F("Execute { "));
        _state = State::Execute;
        Ao_t::_registers[HID_HID_TIMEOUT] = 1;
      }
    }
   
    void stateExecute() {
      if (_state == State::Execute && Ao_t::_registers[HID_HID_TIMEOUT] != 0) {

        _ticksRemaining--;
        _program->play();

        if (_ticksRemaining == 0) {
          _log.println(F("}"));
          _program->rewind();
          _program = 0;
          _state = State::Idle;
          Ao_t::_registers[HID_HID_TIMEOUT] = 0;
        }
      }
    }

  private:
    typedef Ao<HidAo<TLOGGER, TPROGRAM>> Ao_t;
  
    TPROGRAM* _programs;
    TLOGGER& _log;

    enum class State {
      Idle,
      Execute
    };
    State _state = State::Idle;
    TPROGRAM* _program = 0;
    size_t _ticksRemaining = 0;
  };

}
