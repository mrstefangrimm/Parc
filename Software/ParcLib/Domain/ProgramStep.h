// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parclib {

  // Every virtual method has a pointer (two bytes) in the virtual table.
  enum class VirtualAction {
    Dispose,
    Tick
  };
    
  template<class TLOGGER>
  class ProgramStep {
  public:
    ProgramStep(TLOGGER& logger, uint8_t duration);

    void dispose();
    ProgramStep<TLOGGER>* play(uint8_t& tick);
    size_t duration() const;

    ProgramStep<TLOGGER>* appendStep(ProgramStep<TLOGGER>* step);

  protected:
    virtual void action(VirtualAction type, uint8_t& tick) = 0;

  protected:
    TLOGGER& _log;
    ProgramStep* _next;

    uint8_t _duration;

  };


  template<class TLOGGER>
  inline ProgramStep<TLOGGER>::ProgramStep(TLOGGER& logger, uint8_t duration)
    : _log(logger), _duration(duration), _next(0) {
    // Debug: static int numSteps = 0; numSteps++;_log.println(numSteps);
  }

  template<class TLOGGER>
  inline void ProgramStep<TLOGGER>::dispose() {
    if (_next != 0) {
      _next->dispose();
      delete _next;
      _next = 0;
    }
    action(VirtualAction::Dispose, _duration);
  }

  template<class TLOGGER>
  inline ProgramStep<TLOGGER>* ProgramStep<TLOGGER>::play(uint8_t& tick) {
    if (tick < _duration) {
      action(VirtualAction::Tick, tick);
      tick++;
      return this;
    }
    else if (_next != 0) {
      tick = 0;
      _next->play(tick);
    }
    return _next;
  }
  
  template<class TLOGGER>
  inline size_t ProgramStep<TLOGGER>::duration() const {
    if (_next != 0) { return _duration + _next->duration(); }
    else { return _duration; }
  }
  
  template<class TLOGGER>
  inline ProgramStep<TLOGGER>* ProgramStep<TLOGGER>::appendStep(ProgramStep<TLOGGER>* step) {
    if (_next == 0) {
      _next = step; return _next;
    }
    else {
      return _next->appendStep(step);
    }
  }

}
