// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"

namespace parclib {

  template<class TLOGGER, class TSYSTEMHW, uint8_t LOWMEMORY>
  class SystemMonitorAo : public Ao<SystemMonitorAo<TLOGGER, TSYSTEMHW, LOWMEMORY>> {
  public:
    SystemMonitorAo(TLOGGER& logger, /*TSYSTEMHW& monitorHw,*/ RegisterData_t* registers)
      : Ao_t(registers), _log(logger)/*, _monitorHw(monitorHw)*/ {}

    void checkRegisters() {

      if (Ao_t::_registers[TERMINAL_MONITOR_PROGCHANGE] != 0) {
        logMemory();
        Ao_t::_registers[TERMINAL_MONITOR_PROGCHANGE] = 0;
      }

      if (Ao_t::_registers[KEYPAD_MONITOR_WRONGPIN] != 0) {
        PinRegData pinData(Ao_t::_registers[KEYPAD_MONITOR_WRONGPIN]);
        if (pinData.failed > 0) {
          Ao_t::_registers[MONITOR_MONITOR_TIMEOUT] = TimerRegData(5000 / TimerPeriod);
          TSYSTEMHW::warnLedOn();
        }
        else {
          auto freeMem = TSYSTEMHW::freeMemory();
           if (freeMem < LOWMEMORY) {
             TSYSTEMHW::warnLedOn();
           }
           else {
             TSYSTEMHW::warnLedOff();
           }
        }
        Ao_t::_registers[KEYPAD_MONITOR_WRONGPIN] = 0;
      }

      if (Ao_t::_registers[MONITOR_MONITOR_TIMEOUT] > 1) {
        Ao_t::_registers[MONITOR_MONITOR_TIMEOUT]--;
      }
      else if (Ao_t::_registers[MONITOR_MONITOR_TIMEOUT] == 1) {
        logMemory();
        Ao_t::_registers[MONITOR_MONITOR_TIMEOUT] = TimerRegData(25500 / TimerPeriod);
      }
    }

  private:
    typedef Ao<SystemMonitorAo<TLOGGER, TSYSTEMHW, LOWMEMORY>> Ao_t;
        
    void logMemory() {
      auto freeMem = TSYSTEMHW::freeMemory();
      _log.print(F("Free Memory: ")); _log.println(freeMem);
      if (freeMem < LOWMEMORY) {
        TSYSTEMHW::warnLedOn();
      }
      else {
        TSYSTEMHW::warnLedOff();
      }
    }

    TLOGGER& _log;
    // TSYSTEMHW& _monitorHw;
  };

}
