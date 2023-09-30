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

template<class TLOGGERFAC>
class ProgramStep {
  public:
    explicit ProgramStep(uint8_t duration);
    virtual ~ProgramStep() {
      dispose();
    }

    void dispose();
    ProgramStep<TLOGGERFAC>* play(uint8_t& tick);
    size_t duration() const;

    ProgramStep<TLOGGERFAC>* appendStep(ProgramStep<TLOGGERFAC>* step);

  protected:
    virtual void action(VirtualAction type, uint8_t& tick) = 0;

  protected:
    ProgramStep* _next = nullptr;
    uint8_t _duration;

};

template<class TLOGGERFAC>
inline ProgramStep<TLOGGERFAC>::ProgramStep(uint8_t duration)
  : _duration(duration) {
  // Debug: static int numSteps = 0; numSteps++;_log.println(numSteps);
}

template<class TLOGGERFAC>
inline void ProgramStep<TLOGGERFAC>::dispose() {
  if (_next != 0) {
    _next->dispose();
    delete _next;
    _next = 0;
  }
  action(VirtualAction::Dispose, _duration);
}

template<class TLOGGERFAC>
inline ProgramStep<TLOGGERFAC>* ProgramStep<TLOGGERFAC>::play(uint8_t& tick) {
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

template<class TLOGGERFAC>
inline size_t ProgramStep<TLOGGERFAC>::duration() const {
  if (_next != 0) {
    return _duration + _next->duration();
  }
  else {
    return _duration;
  }
}

template<class TLOGGERFAC>
inline ProgramStep<TLOGGERFAC>* ProgramStep<TLOGGERFAC>::appendStep(ProgramStep<TLOGGERFAC>* step) {
  if (_next == 0) {
    _next = step; return _next;
  }
  else {
    return _next->appendStep(step);
  }
}

}
