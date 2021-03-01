// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Registers.h"

namespace parclib {

  template<typename TDERIVED>
  class Ao {
  public:
    Ao(RegisterData_t* registers)
      : _registers(registers) {}

    void checkRegisters() {
      static_cast<TDERIVED*>(this)->checkRegisters();
    }

  protected:
    RegisterData_t* _registers;
  };

}
