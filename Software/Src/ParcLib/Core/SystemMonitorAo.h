// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#ifndef PARCLIB_SYSTEMMONITORAO_H
#define PARCLIB_SYSTEMMONITORAO_H

#include "Ao.h"
#include "BitCounter.h"

namespace parclib {

template<class TLOGGERFAC, class TSYSTEMHWFAC, uint8_t LOWMEMORY>
class SystemMonitorAo : public Ao<SystemMonitorAo<TLOGGERFAC, TSYSTEMHWFAC, LOWMEMORY>> {
  public:
    explicit SystemMonitorAo(Messages& messages)
      : Ao_t(messages), _gameOver(false) {
    }

    void load() {
      if (_timer.increment()) {
        _progChangeMsg = Ao_t::_messages.fromTerminalToServiceMonitorQueue.pop();
        _pinMsg = Ao_t::_messages.fromKeypadToServiceMonitorQueue.pop();
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
            _notificationTimer = NotificationTimer_t(ledOnfor5secTimeout);

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
    using Timer_t = BitCounter<0>;
    using NotificationTimer_t = BitCounter<8>;

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
    MessageData_t _progChangeMsg = 0;
    MessageData_t _pinMsg = 0;
};

}

#endif
