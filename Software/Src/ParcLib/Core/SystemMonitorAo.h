// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "BitTimer.h"

namespace parclib {

template<class TLOGGERFAC, class TSYSTEMHWFAC, uint8_t LOWMEMORY>
class SystemMonitorAo : public Ao<SystemMonitorAo<TLOGGERFAC, TSYSTEMHWFAC, LOWMEMORY>> {
  public:
    explicit SystemMonitorAo(Register* registers)
      : Ao_t(registers), _gameOver(false) {
    }

    void load() {
      if (_timer.increment()) {
        _progChangeMsg = Ao_t::_registers->get(TERMINAL_MONITOR_PROGCHANGE);
        _pinMsg = Ao_t::_registers->get(KEYPAD_MONITOR_PIN);

        Ao_t::_registers->set(TERMINAL_MONITOR_PROGCHANGE, 0);
        Ao_t::_registers->set(KEYPAD_MONITOR_PIN, 0);
      }
    }

    void run() {
      if (_timer.current()) {

        if (_progChangeMsg != 0) {
          logMemoryAndWarn();
        }

        if (_pinMsg != 0) {
          auto sysHw = TSYSTEMHWFAC::create();
          PinRegData pinData(_pinMsg);
          if (pinData.failed > 0) {
            _gameOver = pinData.isGameOver();
            sysHw->warnLedOn();

            const uint8_t ledOnfor5secTimeout = 5000 / TimerPeriod;
            _notificationTimer = NotificationTimer_t((1 << 8) - ledOnfor5secTimeout);

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
        }
        _progChangeMsg = 0;
        _pinMsg = 0;
      }

      if (_notificationTimer.increment()) {
        logMemoryAndWarn();
      }
    }

  private:
    using Ao_t = Ao<SystemMonitorAo<TLOGGERFAC, TSYSTEMHWFAC, LOWMEMORY>>;
    using Timer_t = BitTimer<0>;
    using NotificationTimer_t = BitTimer<8>;

    void logMemoryAndWarn() {
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

    bool _gameOver = false;
    Timer_t _timer;
    NotificationTimer_t _notificationTimer;
    RegisterData_t _progChangeMsg = 0;
    RegisterData_t _pinMsg = 0;
};

}
