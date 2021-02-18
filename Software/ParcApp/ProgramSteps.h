// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "ProgramStep.h"

namespace parc {
    
  template<typename TLOGGER>
  class ProgramStepWait : public ProgramStep<TLOGGER> {
  public:
    ProgramStepWait(TLOGGER& logger, uint16_t waitMs) : ProgramStep<TLOGGER>(logger, waitMs / TimerPeriod) {}
 
    void action(VirtualAction type) {
     if (type == VirtualAction::Tick) {
        onTick();
      }
    }
  
  private:
    void onTick() {
      if (ProgramStep<TLOGGER>::_tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("W "));
      }
    }
  };

  template<typename TLOGGER, typename THIDBLE>
  class ProgramStepBleKeyboardText : public ProgramStep<TLOGGER> {
  public:
    ProgramStepBleKeyboardText(TLOGGER& logger, THIDBLE& ble, const char* text)
      : ProgramStep<TLOGGER>(logger, max(10, strlen(text) * 100 / TimerPeriod)), _ble(ble) {
      auto len = strlen(text) + 1;
      _text = new char[len];
      strncpy(_text, text, len);
    }

    void action(VirtualAction type) {
      if (type == VirtualAction::Dispose) {
        dispose();
      }
      else if (type == VirtualAction::Tick) {
        onTick();
      }
    }

  private:
    void dispose() {
      if (ProgramStep<TLOGGER>::_next != 0) {
        ProgramStep<TLOGGER>::_next->action(VirtualAction::Dispose);
        delete ProgramStep<TLOGGER>::_next;
        ProgramStep<TLOGGER>::_next = 0;
      }
      delete[] _text;
      _text = 0;
    }

    void onTick() {
      if (ProgramStep<TLOGGER>::_tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("BT "));
        //ProgramStep<TLOGGER>::_log.println(_text);

        _ble.print("AT+BleKeyboard=");
        _ble.println(_text);

        if (_ble.waitForOK()) { return; }
        ProgramStep<TLOGGER>::_log.println(F("Send keyboard text failed. Enable verbose mode for more information."));
      }
    }

  private:
    THIDBLE& _ble;
    char* _text;
  };

  template<typename TLOGGER, typename THIDBLE>
  class ProgramStepBleKeyboardCode : public ProgramStep<TLOGGER> {
  public:
    ProgramStepBleKeyboardCode(TLOGGER& logger, THIDBLE& ble, KeyCode keyCode)
      : ProgramStep<TLOGGER>(logger, 5), _ble(ble), _keyCode(keyCode) {}

  protected:
    void onTick() {
      if (ProgramStep<TLOGGER>::_tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("BK "));
        //ProgramStep<TLOGGER>::_log.println(_hexCode);

        _ble.sendKeyCode(_keyCode);
      }
    }

  private:
    THIDBLE& _ble;
    KeyCode _keyCode;
  };

  template<typename TLOGGER, typename THIDBLE>
  class ProgramStepBleControlKey : public ProgramStep<TLOGGER> {
  public:
    ProgramStepBleControlKey(TLOGGER& logger, THIDBLE& ble, const char* ctrlKey, uint16_t duration)
      : ProgramStep<TLOGGER>(logger, max(1, duration / TimerPeriod)), _ble(ble) {
      auto len = strlen(ctrlKey) + 1;
      _ctrlKey = new char[len];
      strncpy(_ctrlKey, ctrlKey, len);
    }

    void action(VirtualAction type) {
      if (type == VirtualAction::Dispose) {
        dispose();
      }
      else if (type == VirtualAction::Tick) {
        onTick();
      }
    }

  private:
    void dispose() {
      if (ProgramStep<TLOGGER>::_next != 0) {
        ProgramStep<TLOGGER>::_next->action(VirtualAction::Dispose);
        delete ProgramStep<TLOGGER>::_next;
        ProgramStep<TLOGGER>::_next = 0;
      }
      delete[] _ctrlKey;
      _ctrlKey = 0;
    }

    void onTick() {
      if (ProgramStep<TLOGGER>::_tick == 0) {
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

  template<typename TLOGGER, typename THIDUSB>
  class ProgramStepUsbKeyboardText : public ProgramStep<TLOGGER> {
  public:
    ProgramStepUsbKeyboardText(TLOGGER& logger, THIDUSB& usb, const char* text)
      : ProgramStep<TLOGGER>(logger, strlen(text) * 100 / TimerPeriod), _usb(usb) {
      auto len = strlen(text) + 1;
      _text = new char[len];
      strncpy(_text, text, len);
    }

    void action(VirtualAction type) {
      if (type == VirtualAction::Dispose) {
        dispose();
      }
      else if (type == VirtualAction::Tick) {
        onTick();
      }
    }

  private:
    void dispose() {
      if (ProgramStep<TLOGGER>::_next != 0) {
        ProgramStep<TLOGGER>::_next->action(VirtualAction::Dispose);
        delete ProgramStep<TLOGGER>::_next;
        ProgramStep<TLOGGER>::_next = 0;
      }
      delete[] _text;
      _text = 0;
    }

    void onTick() {
      if (ProgramStep<TLOGGER>::_tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("UT "));
        _usb.println(_text);
      }
    }

  private:
    THIDUSB& _usb;
    char* _text;
  };

  template<typename TLOGGER, typename THIDUSB>
  class ProgramStepUsbKeyboardCode : public ProgramStep<TLOGGER> {
  public:
    ProgramStepUsbKeyboardCode(TLOGGER& logger, THIDUSB& usb, KeyCode keyCode)
      : ProgramStep<TLOGGER>(logger, 1), _usb(usb), _keyCode(keyCode) {}

    void action(ProgramStep<TLOGGER> type) {
      if (type == ProgramStep<TLOGGER>::Tick) {
        onTick();
      }
    }

  private:
    void onTick() {
      if (ProgramStep<TLOGGER>::_tick == 0) {
        ProgramStep<TLOGGER>::_log.print(F("UK "));
        //ProgramStep<TSERIAL>::_log.println(_ctrl);
        //ProgramStep<TSERIAL>::_log.println(_shift);
        //ProgramStep<TSERIAL>::_log.println(_alt);
        //ProgramStep<TSERIAL>::_log.println(_win);
        //ProgramStep<TSERIAL>::_log.println(_hexCode);
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
