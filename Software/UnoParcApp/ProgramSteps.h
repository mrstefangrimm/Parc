// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Src/ProgramStep.h"

namespace unoparc {

  template<typename TLOGGER, typename TDERIVED, bool DISPOSABLE>
  class ProgramStepBase : public ProgramStep<TLOGGER> {
  public:
    ProgramStepBase(TLOGGER& logger, uint8_t duration)
      : ProgramStep<TLOGGER>(logger, duration) {}

  protected:
    void action(VirtualAction type) final {
      action(type, Int2Type<DISPOSABLE>());
    }

  private:
    void action(VirtualAction type, Int2Type<true>) {
      if (type == VirtualAction::Dispose) {
        static_cast<TDERIVED*>(this)->doDispose();
      }
      else if (type == VirtualAction::Tick) {
        static_cast<TDERIVED*>(this)->doTick();
      }
    }

    void action(VirtualAction type, Int2Type<false>) {
      if (type == VirtualAction::Tick) {
        static_cast<TDERIVED*>(this)->doTick();
      }
    }

  };
      
  template<typename TLOGGER>
  class ProgramStepWait : public ProgramStepBase<TLOGGER, ProgramStepWait<TLOGGER>, false> {
  public:
    ProgramStepWait(TLOGGER& logger, uint16_t waitMs) : ProgramStepBase<TLOGGER, ProgramStepWait<TLOGGER>, false>(logger, waitMs / TimerPeriod) {}
   
    void doTick() {
      if (ProgramStep<TLOGGER>::_tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("W "));
      }
    }
  };

  template<typename TLOGGER, typename THIDUSB>
  class ProgramStepUsbKeyboardCode : public ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardCode<TLOGGER, THIDUSB>, false> {
  public:
    static const uint8_t Radix = 10;
    static const uint8_t KeyCodeDel = 76;   // 0x4C
    static const uint8_t KeyCodeTab = 43;   // 0x2B
    static const uint8_t KeyCodeEnter = 40; // 0x28
    static const uint8_t KeyCodeSpace = 44; // 0x2C
  
    ProgramStepUsbKeyboardCode(TLOGGER& logger, THIDUSB& usb, KeyCode keyCode)
      : ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardCode<TLOGGER, THIDUSB>, false>(logger, 1), _usb(usb), _keyCode(keyCode) {}
       
    void doTick() {
      if (Ps_t::_tick == 0) {
        Ps_t::_log.print(F("UK "));

        // Debug
        Ps_t::_log.println();
        Ps_t::_log.println(_keyCode.ctrl);
        Ps_t::_log.println(_keyCode.shift);
        Ps_t::_log.println(_keyCode.alt);
        Ps_t::_log.println(_keyCode.win);
        Ps_t::_log.println(_keyCode.hexCode);
        
        uint8_t pos = 2;
        if (_keyCode.ctrl) { _buf[pos] = 224; pos++; }
        if (_keyCode.shift) { _buf[pos] = 225; pos++; }
        if (_keyCode.alt) { _buf[pos] = 226; pos++; }
        if (_keyCode.win) { _buf[pos] = 227; pos++; }
        
        _buf[pos] = _keyCode.hexCode;
        _usb.write(_buf, sizeof(_buf));
        releaseKey();
      }
    }

  private:
    typedef ProgramStep<TLOGGER> Ps_t;

    void releaseKey() {
      memset(_buf, 0, sizeof(_buf));
      _usb.write(_buf, 8);
    }

    uint8_t _buf[8] = { 0 };
    THIDUSB& _usb;
    KeyCode _keyCode;
  };

}
