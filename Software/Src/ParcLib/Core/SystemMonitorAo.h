// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"

namespace parclib {

template<class TLOGGERFAC, class TSYSTEMHWFAC, uint8_t LOWMEMORY>
class SystemMonitorAo : public Ao<SystemMonitorAo<TLOGGERFAC, TSYSTEMHWFAC, LOWMEMORY>> {
  public:
    SystemMonitorAo(RegisterData_t* registers)
      : Ao_t(registers), _gameOver(false) {
    }

    void checkRegisters() {

      if (Ao_t::_registers[TERMINAL_MONITOR_PROGCHANGE] != 0) {
        logMemory();
        Ao_t::_registers[TERMINAL_MONITOR_PROGCHANGE] = 0;
      }

      if (Ao_t::_registers[KEYPAD_MONITOR_WRONGPIN] != 0) {
        auto sysHw = TSYSTEMHWFAC::create();
        PinRegData pinData(Ao_t::_registers[KEYPAD_MONITOR_WRONGPIN]);
        if (pinData.failed > 0) {
          _gameOver = pinData.isGameOver();
          Ao_t::_registers[MONITOR_MONITOR_TIMEOUT] = TimerRegData(5000 / TimerPeriod);
          sysHw->warnLedOn();
        }
        else {
          auto freeMem = sysHw->freeMemory();
          if (freeMem < LOWMEMORY) {
            sysHw->warnLedOn();
          }
          else {
            sysHw->warnLedOff();
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
    typedef Ao<SystemMonitorAo<TLOGGERFAC, TSYSTEMHWFAC, LOWMEMORY>> Ao_t;

    void logMemory() {
      auto log = TLOGGERFAC::create();
      auto sysHw = TSYSTEMHWFAC::create();
      auto freeMem = sysHw->freeMemory();
      log->print(F("Free Memory: ")); log->println(freeMem);
      if (freeMem < LOWMEMORY || _gameOver) {
        sysHw->warnLedOn();
      }
      else {
        sysHw->warnLedOff();
      }
    }

    bool _gameOver;
};

}
