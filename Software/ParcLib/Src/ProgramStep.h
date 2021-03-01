// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parclib {

  // Saves 12 bytes. Every virtual method has a pointer (two bytes) in the virtual table, i.e per class.
  enum class VirtualAction {
    Dispose,
    Tick
  };
    
  template<typename TLOGGER>
  class ProgramStep {
  public:
    ProgramStep(TLOGGER& logger, uint8_t duration);

    void dispose();
    void play();
    void rewind();
    size_t duration() const;

    ProgramStep<TLOGGER>* appendStep(ProgramStep<TLOGGER>* step);

  protected:
    virtual void action(VirtualAction type) = 0;

  protected:
    TLOGGER& _log;
    ProgramStep* _next;

    uint8_t _tick;
    uint8_t _duration;
  };
  
  template<typename TLOGGER>
  inline ProgramStep<TLOGGER>::ProgramStep(TLOGGER& logger, uint8_t duration)
    : _log(logger), _duration(duration), _tick(0), _next(0) {}

  template<typename TLOGGER>
  inline void ProgramStep<TLOGGER>::dispose() {
    if (_next != 0) {
      _next->dispose();
      delete _next;
      _next = 0;
    }
    action(VirtualAction::Dispose);
  }

  template<typename TLOGGER>
  inline void ProgramStep<TLOGGER>::play() {
    if (_tick < _duration) {
      action(VirtualAction::Tick);
      _tick++;
    }
    else if (_next != 0) {
      _next->play();
    }
  }

  template<typename TLOGGER>
  inline void ProgramStep<TLOGGER>::rewind() {
    _tick = 0;
    if (_next != 0) { _next->rewind(); }
  }

  template<typename TLOGGER>
  inline size_t ProgramStep<TLOGGER>::duration() const {
    if (_next != 0) { return _duration + _next->duration(); }
    else { return _duration; }
  }
  
  template<typename TLOGGER>
  inline ProgramStep<TLOGGER>* ProgramStep<TLOGGER>::appendStep(ProgramStep<TLOGGER>* step) {
    if (_next == 0) {
      _next = step; return _next;
    }
    else {
      return _next->appendStep(step);
    }
  }

}
