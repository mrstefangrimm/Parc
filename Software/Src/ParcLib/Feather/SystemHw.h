// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "FreeMemory.h"

namespace parclib {

struct SystemHw {
  static int freeMemory() {
    return ::freeMemory();
  }
  static void warnLedOn() {
    ::digitalWrite(LED_BUILTIN, HIGH);
  }
  static void warnLedOff() {
    ::digitalWrite(LED_BUILTIN, LOW);
  }
};

}
