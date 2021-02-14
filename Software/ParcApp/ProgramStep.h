// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parc {

  template<typename TLOGGER>
  class ProgramStep {
  public:
    ProgramStep(TLOGGER& logger, uint8_t length);

    virtual void dispose();

    void play();

    void rewind();

    //virtual void cancel() = 0;
    void setNext(ProgramStep* next);

    size_t duration();
    void appendStep(ProgramStep<TLOGGER>* step);

  protected:
    virtual void onTick() = 0;

    TLOGGER& _log;

    uint8_t _tick;
    uint8_t _duration;
    ProgramStep* _next;
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
  }

  template<typename TLOGGER>
  inline void ProgramStep<TLOGGER>::play() {
    if (_tick < _duration) {
      onTick();
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

  //virtual void cancel() = 0;
  template<typename TLOGGER>
  inline void ProgramStep<TLOGGER>::setNext(ProgramStep* next) { _next = next; }

  template<typename TLOGGER>
  inline size_t ProgramStep<TLOGGER>::duration() {
    if (_next != 0) { return _duration + _next->duration(); }
    else { return _duration; }
  }
  
  template<typename TLOGGER>
  inline void ProgramStep<TLOGGER>::appendStep(ProgramStep<TLOGGER>* step) {
    if (_next == 0) {
      _next = step;
    }
    else {
      _next->appendStep(step);
    }
  }

}
