// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "FreeMemory.h"



namespace parclib {

  template<class TLOGGER, uint8_t LOWMEMORY>
  class MemoryMonitorAo : public Ao<MemoryMonitorAo<TLOGGER, LOWMEMORY>> {
  public:
    MemoryMonitorAo(TLOGGER& logger, RegisterData_t* registers)
      : Ao_t(registers), _log(logger) {}

    void checkRegisters() {

      if (Ao_t::_registers[TERMINAL_MEMORY_CHANGE] != 0) {
        logMemory();
        Ao_t::_registers[TERMINAL_MEMORY_CHANGE] = 0;
      }

      if (Ao_t::_registers[KEYPAD_MEMORY_WRONG] != 0) {
        PinRegData pinData(Ao_t::_registers[KEYPAD_MEMORY_WRONG]);
        if (pinData.failed > 0) {
          digitalWrite(LED_BUILTIN, HIGH);
        }
        else {
          auto freeMem = freeMemory();
           if (freeMem < LOWMEMORY) {
             digitalWrite(LED_BUILTIN, HIGH);
           }
           else {
             digitalWrite(LED_BUILTIN, LOW);
           }
        }
        Ao_t::_registers[KEYPAD_MEMORY_WRONG] = 0;
      }

      if (Ao_t::_registers[MEMORY_MEMORY_TIMEOUT] > 1) {
        Ao_t::_registers[MEMORY_MEMORY_TIMEOUT]--;
      }
      else if (Ao_t::_registers[MEMORY_MEMORY_TIMEOUT] == 1) {
        logMemory();
        Ao_t::_registers[MEMORY_MEMORY_TIMEOUT] = TimerRegData(60000 / TimerPeriod);
      }
    }

  private:
    typedef Ao<MemoryMonitorAo<TLOGGER, LOWMEMORY>> Ao_t;

    
    void logMemory() {
      auto freeMem = freeMemory();
      _log.print(F("Free Memory: ")); _log.println(freeMem);
      if (freeMem < LOWMEMORY) {
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }

    TLOGGER& _log;
  };

}
