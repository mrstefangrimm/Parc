// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "ProgramStep.h"

namespace parclib {

template<class TLOGGERFAC>
class Program {
  public:
    size_t duration() const {
      if (_root != 0) {
        return _root->duration();
      }
      else {
        return 0;
      }
    }

    ProgramStep<TLOGGERFAC>* appendStep(ProgramStep<TLOGGERFAC>* step) {
      if (_root == 0) {
        _root = step;
        _currentStep = step;
        return _root;
      }
      else {
        return _root->appendStep(step);
      }
    }

    void dispose() {
      if (_root != 0) {
        _root->dispose();
        delete _root;
        _root = 0;
      }
    }

    void play() {
      if (_currentStep != 0) {
        _currentStep = _currentStep->play(_tick);
      }
    }

    void rewind() {
      _tick = 0;
      _currentStep = _root;
    }

    bool hasSteps() const {
      return _root != 0;
    }

  private:
    ProgramStep<TLOGGERFAC>* _root = 0;
    ProgramStep<TLOGGERFAC>* _currentStep = 0;
    uint8_t _tick = 0;
};

}
