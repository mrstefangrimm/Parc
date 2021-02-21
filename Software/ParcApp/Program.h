// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "ProgramStep.h"

namespace parc {
  
  template<typename TLOGGER>
  class Program {
  public:
    size_t duration() const {
      if (_root != 0) { return _root->duration(); }
      else { return 0; }
    }

    ProgramStep<TLOGGER>* appendStep(ProgramStep<TLOGGER>* step) {
      if (_root == 0) { _root = step; return _root; }
      else { return _root->appendStep(step); }
    }

    void dispose() {
      if (_root != 0) {
        _root->dispose();
        delete _root;
        _root = 0;
      }
    }

    void play() const {
      if (_root != 0) {
        _root->play();
      }
    }

    void rewind() const {
      if (_root != 0) {
        _root->rewind();
      }
    }

    bool hasSteps() const {
      return _root != 0;
    }

  private:
    ProgramStep<TLOGGER>* _root = 0;
  };

}
