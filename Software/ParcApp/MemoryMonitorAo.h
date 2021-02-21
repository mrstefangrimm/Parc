// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "FreeMemory.h"

namespace parc {

  template<typename TLOGGER>
  class MemoryMonitorAo : public Ao<MemoryMonitorAo<TLOGGER>> {
  public:
    MemoryMonitorAo(TLOGGER& logger, RegisterData_t* registers)
      : Ao<MemoryMonitorAo<TLOGGER>>(registers), _log(logger) {}

    void checkRegisters() {

      if (Ao<MemoryMonitorAo<TLOGGER>>::_registers[TERMINAL_MEMORY_CHANGE] != 0) {
        logMemory();
        Ao<MemoryMonitorAo<TLOGGER>>::_registers[TERMINAL_MEMORY_CHANGE] = 0;
      }

      if (Ao<MemoryMonitorAo<TLOGGER>>::_registers[KEYPAD_MEMORY_WRONG] != 0) {
        PinRegData pinData(Ao<MemoryMonitorAo<TLOGGER>>::_registers[KEYPAD_MEMORY_WRONG]);
        if (pinData.failed > 0) {
          digitalWrite(LED_BUILTIN, HIGH);
        }
        else {
          auto freeMem = freeMemory();
           if (freeMem < 200) {
             digitalWrite(LED_BUILTIN, HIGH);
           }
           else {
             digitalWrite(LED_BUILTIN, LOW);
           }
        }
        Ao<MemoryMonitorAo<TLOGGER>>::_registers[KEYPAD_MEMORY_WRONG] = 0;
      }

      if (Ao<MemoryMonitorAo<TLOGGER>>::_registers[MEMORY_MEMORY_TIMEOUT] > 1) {
        Ao<MemoryMonitorAo<TLOGGER>>::_registers[MEMORY_MEMORY_TIMEOUT]--;
      }
      else if (Ao<MemoryMonitorAo<TLOGGER>>::_registers[MEMORY_MEMORY_TIMEOUT] == 1) {
        logMemory();
        Ao<MemoryMonitorAo<TLOGGER>>::_registers[MEMORY_MEMORY_TIMEOUT] = TimerRegData(60000 / TimerPeriod);
      }
    }

  private:
    void logMemory() {
      auto freeMem = freeMemory();
      _log.print(F("Free Memory: ")); _log.println(freeMem);
      if (freeMem < 200) {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }

    TLOGGER& _log;
  };

}
