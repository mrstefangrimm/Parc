// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Src/ProgramStep.h"

namespace pcbparc {

  using namespace parclib;

  template<class TLOGGER, class TDERIVED, bool DISPOSABLE>
  class ProgramStepBase : public ProgramStep<TLOGGER> {
  public:
    ProgramStepBase(TLOGGER& logger, uint8_t duration)
      : ProgramStep<TLOGGER>(logger, duration) {}

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
      
  template<class TLOGGER>
  class ProgramStepWait : public ProgramStepBase<TLOGGER, ProgramStepWait<TLOGGER>, false> {
  public:
    ProgramStepWait(TLOGGER& logger, uint16_t waitMs) : ProgramStepBase<TLOGGER, ProgramStepWait<TLOGGER>, false>(logger, waitMs / TimerPeriod) {}
   
    void doTick(uint8_t tick) {
      if (tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("W "));
      }
    }
  };

  template<class TLOGGER, class THIDBLE>
  class ProgramStepBleKeyboardText : public ProgramStepBase<TLOGGER, ProgramStepBleKeyboardText<TLOGGER, THIDBLE>, true> {
  public:
    ProgramStepBleKeyboardText(TLOGGER& logger, THIDBLE& ble, const char* text)
      : ProgramStepBase<TLOGGER, ProgramStepBleKeyboardText<TLOGGER, THIDBLE>, true>(logger, max(10, strlen(text) * 100 / TimerPeriod)), _ble(ble) {
      auto len = strlen(text) + 1;
      _text = new char[len];
      strncpy(_text, text, len);
    }
     
    void doDispose() {
      delete[] _text;
      _text = 0;
    }

    void doTick(uint8_t tick) {
      if (tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("BT "));
        //ProgramStep<TLOGGER>::_log.println(_text);

        _ble.print(F("AT+BleKeyboard="));
        _ble.println(_text);

        if (_ble.waitForOK()) { return; }
        ProgramStep<TLOGGER>::_log.println(F("Send keyboard text failed. Enable verbose mode for more information."));
      }
    }

  private:
    THIDBLE& _ble;
    char* _text;
  };

  template<class TLOGGER, class THIDBLE>
  class ProgramStepBleKeyboardCodeRepeated : public ProgramStepBase<TLOGGER, ProgramStepBleKeyboardCodeRepeated<TLOGGER, THIDBLE>, false> {
  public:
    static const uint8_t Radix = 16;
    static const uint8_t KeyCodeDel = 0x4C;
    static const uint8_t KeyCodeTab = 0x2B;
    static const uint8_t KeyCodeEnter = 0x28;
    static const uint8_t KeyCodeSpace = 0x2C;

    ProgramStepBleKeyboardCodeRepeated(TLOGGER& logger, THIDBLE& ble, KeyCode keyCode, uint8_t numRepetitions)
      : ProgramStepBase<TLOGGER, ProgramStepBleKeyboardCodeRepeated<TLOGGER, THIDBLE>, false>(logger, 5 * numRepetitions), _ble(ble), _keyCode(keyCode) {}

    void doTick(uint8_t tick) {
      if (tick % 5 == 0) {
        ProgramStep<TLOGGER>::_log.print(F("BK "));
        //ProgramStep<TLOGGER>::_log.println(_hexCode);

        _ble.sendKeyCode(_keyCode);
      }
    }

  private:
    THIDBLE& _ble;
    KeyCode _keyCode;
  };

  template<class TLOGGER, class THIDBLE>
  class ProgramStepBleKeyboardCode : public ProgramStepBase<TLOGGER, ProgramStepBleKeyboardCode<TLOGGER, THIDBLE>, false> {
  public:
    static const uint8_t Radix = 16;
    static const uint8_t KeyCodeDel = 0x4C;
    static const uint8_t KeyCodeTab = 0x2B;
    static const uint8_t KeyCodeEnter = 0x28;
    static const uint8_t KeyCodeSpace = 0x2C;
              
    ProgramStepBleKeyboardCode(TLOGGER& logger, THIDBLE& ble, KeyCode keyCode)
      : ProgramStepBase<TLOGGER, ProgramStepBleKeyboardCode<TLOGGER, THIDBLE>, false>(logger, 5), _ble(ble), _keyCode(keyCode) {}
  
    void doTick(uint8_t tick) {
      if (tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("BK "));
        //ProgramStep<TLOGGER>::_log.println(_hexCode);

        _ble.sendKeyCode(_keyCode);
      }
    }

  private:
    THIDBLE& _ble;
    KeyCode _keyCode;
  };

  template<class TLOGGER, class THIDBLE>
  class ProgramStepBleControlKey : public ProgramStepBase<TLOGGER, ProgramStepBleControlKey<TLOGGER, THIDBLE>, true> {
  public:
    ProgramStepBleControlKey(TLOGGER& logger, THIDBLE& ble, const char* ctrlKey, uint8_t duration)
      : ProgramStepBase<TLOGGER, ProgramStepBleControlKey<TLOGGER, THIDBLE>, true>(logger, max(1, duration / TimerPeriod)), _ble(ble) {
      auto len = strlen(ctrlKey) + 1;
      _ctrlKey = new char[len];
      strncpy(_ctrlKey, ctrlKey, len);
    }

    void doDispose() {
      delete[] _ctrlKey;
      _ctrlKey = 0;
    }

    void doTick(uint8_t tick) {
      if (tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("BC "));
        //ProgramStep<TLOGGER>::_log.println(_ctrlKey);

        _ble.print(F("AT+BleHidControlKey="));
        _ble.println(_ctrlKey);

        if (_ble.waitForOK()) { return; }
        ProgramStep<TLOGGER>::_log.println(F("Send keyboard text failed. Enable verbose mode for more information."));
      }
    }

  private:
    THIDBLE& _ble;
    char* _ctrlKey;
  };

  template<class TLOGGER, class THIDUSB>
  class ProgramStepUsbKeyboardText : public ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardText<TLOGGER, THIDUSB>, true> {
  public:
    ProgramStepUsbKeyboardText(TLOGGER& logger, THIDUSB& usb, const char* text)
      : ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardText<TLOGGER, THIDUSB>, true>(logger, strlen(text) * 100 / TimerPeriod), _usb(usb) {
      auto len = strlen(text) + 1;
      _text = new char[len];
      strncpy(_text, text, len);
    }

    void doDispose() {
      delete[] _text;
      _text = 0;
    }

    void doTick(uint8_t tick) {
      if (tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("UT "));
        _usb.println(_text);
      }
    }

  private:
    THIDUSB& _usb;
    char* _text;
  };

  template<class TLOGGER, class THIDUSB>
  class ProgramStepUsbKeyboardCode : public ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardCode<TLOGGER, THIDUSB>, false> {
  public:
    static const uint8_t Radix = 16;
    static const uint8_t KeyCodeDel = 0xD4;
    static const uint8_t KeyCodeTab = 0xB3;
    static const uint8_t KeyCodeEnter = 0xB0;
    static const uint8_t KeyCodeSpace = ' ';

    ProgramStepUsbKeyboardCode(TLOGGER& logger, THIDUSB& usb, KeyCode keyCode)
      : ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardCode<TLOGGER, THIDUSB>, false>(logger, 1), _usb(usb), _keyCode(keyCode) {}
       
    void doTick(uint8_t tick) {
      if (tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("UK "));
        //ProgramStep<TLOGGER>::_log.println(_keyCode.ctrl);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.shift);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.alt);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.win);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.hexCode);
        if (_keyCode.ctrl) { _usb.press(0x80); }
        if (_keyCode.shift) { _usb.press(0x81); }
        if (_keyCode.alt) { _usb.press(0x82); }
        if (_keyCode.win) { _usb.press(0x83); }
        _usb.press(_keyCode.hexCode);
        _usb.releaseAll();
      }
    }

  private:
    THIDUSB& _usb;
    KeyCode _keyCode;
  };

  template<class TLOGGER, class THIDUSB>
  class ProgramStepUsbKeyboardCodeRepeated : public ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardCodeRepeated<TLOGGER, THIDUSB>, false> {
  public:
    static const uint8_t Radix = 16;
    static const uint8_t KeyCodeDel = 0xD4;
    static const uint8_t KeyCodeTab = 0xB3;
    static const uint8_t KeyCodeEnter = 0xB0;
    static const uint8_t KeyCodeSpace = ' ';

    ProgramStepUsbKeyboardCodeRepeated(TLOGGER& logger, THIDUSB& usb, KeyCode keyCode, uint8_t numRepetitions)
      : ProgramStepBase<TLOGGER, ProgramStepUsbKeyboardCodeRepeated<TLOGGER, THIDUSB>, false>(logger, numRepetitions), _usb(usb), _keyCode(keyCode) {}

    void doTick(uint8_t tick) {
      // _duration is equal to number of repetitions
      if (tick < ProgramStep<TLOGGER>::_duration) {
        ProgramStep<TLOGGER>::_log.print(F("UK "));
        //ProgramStep<TLOGGER>::_log.println(_keyCode.ctrl);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.shift);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.alt);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.win);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.hexCode);
        if (_keyCode.ctrl) { _usb.press(0x80); }
        if (_keyCode.shift) { _usb.press(0x81); }
        if (_keyCode.alt) { _usb.press(0x82); }
        if (_keyCode.win) { _usb.press(0x83); }
        _usb.press(_keyCode.hexCode);
        _usb.releaseAll();
      }
    }

  private:
    THIDUSB& _usb;
    KeyCode _keyCode;
  };

}
