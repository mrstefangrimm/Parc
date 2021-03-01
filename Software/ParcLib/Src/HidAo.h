// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "ProgramStep.h"
#include "Program.h"

namespace parclib {
  
  template<typename TLOGGER>
  class HidAo : public Ao<HidAo<TLOGGER>> {

  public:
    HidAo(TLOGGER& logger, RegisterData_t* registers, Program<TLOGGER>* programs)
      : Ao_t(registers), _log(logger), _programs(programs) {
      //  _log.println(freeMemory());
    }
    
    Program<TLOGGER>* programs() { return _programs; }

    void checkRegisters() {
      switch (_state) {
        case State::Idle: stateIdle(); break;
        case State::Execute: stateExecute(); break;
      };
      Ao_t::_registers[KEYPAD_HID_INPUT] = 0;
    }
    
  private:
    void stateIdle() {
      if (Ao_t::_registers[KEYPAD_HID_INPUT] != 0) {

        KeypadRegData args(Ao_t::_registers[KEYPAD_HID_INPUT]);
        uint8_t progIdx = args.programIndex();
        _program = &_programs[progIdx];
        size_t len = _program->duration();

        _log.print(F("HidAo:stateIdle() Received Profile: ")); _log.print(args.profile); _log.print(F(" Button: ")); _log.print(args.button); 
        _log.print(F(" Prog. ")); _log.print(progIdx); _log.print(F(" Length: ")); _log.println(len);

        _log.print(F("Execute { "));
        _state = State::Execute;
        Ao_t::_registers[HID_HID_TIMEOUT] = TimerRegData(len);
      }
    }
   
    void stateExecute() {

      if (Ao_t::_registers[HID_HID_TIMEOUT] > 0) {
        Ao_t::_registers[HID_HID_TIMEOUT] -= 1;
        //_log.println(_registers[HID_HID_TIMEOUT]);

        if (_program != 0) {
          _program->play();
        }
        else {
          _log.println(F("E: wrong state."));
        }
      }
      else {
        _log.println(F("}"));
        _program->rewind();
        _program = 0;
        _state = State::Idle;
      }
    }

  private:
    typedef Ao<HidAo<TLOGGER>> Ao_t;
  
    Program<TLOGGER>* _programs;
    TLOGGER& _log;

    enum class State {
      Idle,
      Execute
    };
    State _state = State::Idle;
    Program<TLOGGER>* _program = 0;
  };

}
