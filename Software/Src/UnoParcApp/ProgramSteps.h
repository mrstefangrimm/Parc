// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/ProgramStep.h"

namespace unoparc {

template<class TLOGGERFAC, class TDERIVED, bool DISPOSABLE>
class ProgramStepBase : public ProgramStep<TLOGGERFAC> {
  public:
    ProgramStepBase(uint8_t duration)
      : ProgramStep<TLOGGERFAC>(duration) {}

  protected:
    void action(VirtualAction type, uint8_t& tick) final {
      action(type, tick, Int2Type<DISPOSABLE>());
    }

  private:
    void action(VirtualAction type, uint8_t tick, Int2Type<true>) {
      if (type == VirtualAction::Dispose) {
        static_cast<TDERIVED*>(this)->doDispose();
      }
      else if (type == VirtualAction::Tick) {
        static_cast<TDERIVED*>(this)->doTick(tick);
      }
    }

    void action(VirtualAction type, uint8_t tick, Int2Type<false>) {
      if (type == VirtualAction::Tick) {
        static_cast<TDERIVED*>(this)->doTick(tick);
      }
    }

};

template<class TLOGGERFAC>
class ProgramStepWait : public ProgramStepBase<TLOGGERFAC, ProgramStepWait<TLOGGERFAC>, false> {
  public:
    ProgramStepWait(uint16_t waitMs) : ProgramStepBase<TLOGGERFAC, ProgramStepWait<TLOGGERFAC>, false>(waitMs / TimerPeriod) {}

    void doTick(uint8_t tick) {
      if (tick == 0) {
        auto log = TLOGGERFAC::create();
        log->print(F("W "));
      }
    }
};

template<class TLOGGERFAC, class THIDUSBFAC>
class ProgramStepUsbKeyboardCode : public ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCode<TLOGGERFAC, THIDUSBFAC>, false> {
  public:
    ProgramStepUsbKeyboardCode(KeyCode keyCode)
      : ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCode<TLOGGERFAC, THIDUSBFAC>, false>(1), _keyCode(keyCode) {}

    void doTick(uint8_t tick) {
      if (tick == 0) {
        auto log = TLOGGERFAC::create();
        auto usb = THIDUSBFAC::create();
        log->print(F("UK "));

        // Debug
        log->println();
        log->println(_keyCode.ctrl);
        log->println(_keyCode.shift);
        log->println(_keyCode.alt);
        log->println(_keyCode.win);
        log->println(_keyCode.hexCode);

        uint8_t pos = 2;
        if (_keyCode.ctrl) {
          _buf[pos] = 224;
          pos++;
        }
        if (_keyCode.shift) {
          _buf[pos] = 225;
          pos++;
        }
        if (_keyCode.alt) {
          _buf[pos] = 226;
          pos++;
        }
        if (_keyCode.win) {
          _buf[pos] = 227;
          pos++;
        }

        _buf[pos] = _keyCode.hexCode;
        usb->write(_buf, sizeof(_buf));
        releaseKey();
      }
    }

  private:
    void releaseKey() {
      memset(_buf, 0, sizeof(_buf));
      auto usb = THIDUSBFAC::create();
      usb->write(_buf, 8);
    }

    uint8_t _buf[8] = { 0 };
    KeyCode _keyCode;
};
}
