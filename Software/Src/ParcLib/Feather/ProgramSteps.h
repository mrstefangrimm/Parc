// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#ifndef PARCLIB_PROGRAMSTEPS_H
#define PARCLIB_PROGRAMSTEPS_H

#pragma warning(disable:4996)

#include "Core/ProgramStep.h"

namespace parclib {

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

template<class TLOGGERFAC, class THIDBLEFAC>
class ProgramStepBleKeyboardText : public ProgramStepBase<TLOGGERFAC, ProgramStepBleKeyboardText<TLOGGERFAC, THIDBLEFAC>, true> {
  public:
    ProgramStepBleKeyboardText(const char* text)
      : ProgramStepBase<TLOGGERFAC, ProgramStepBleKeyboardText<TLOGGERFAC, THIDBLEFAC>, true>(max(10, strlen(text) * 100 / TimerPeriod)) {
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
        auto log = TLOGGERFAC::create();
        log->print(F("BT "));
        //ProgramStep<TLOGGER>::_log.println(_text);
        auto ble = THIDBLEFAC::create();
        ble->print(F("AT+BleKeyboard="));
        ble->println(_text);

        if (ble->waitForOK()) {
          return;
        }
        log->println(F("Send keyboard text failed. Enable verbose mode for more information."));
      }
    }

  private:
    char* _text;
};

template<class TLOGGERFAC, class THIDBLEFAC>
class ProgramStepBleKeyboardCode : public ProgramStepBase<TLOGGERFAC, ProgramStepBleKeyboardCode<TLOGGERFAC, THIDBLEFAC>, false> {
  public:
    ProgramStepBleKeyboardCode(KeyCode keyCode)
      : ProgramStepBase<TLOGGERFAC, ProgramStepBleKeyboardCode<TLOGGERFAC, THIDBLEFAC>, false>(5), _keyCode(keyCode) {}

    void doTick(uint8_t tick) {
      if (tick == 0) {
        auto log = TLOGGERFAC::create();
        log->print(F("BK "));
        //ProgramStep<TLOGGER>::_log.println(_hexCode);
        auto ble = THIDBLEFAC::create();
        ble->sendKeyCode(_keyCode);
      }
    }

  private:
    KeyCode _keyCode;
};

template<class TLOGGERFAC, class THIDBLEFAC>
class ProgramStepBleKeyboardCodeRepeated : public ProgramStepBase<TLOGGERFAC, ProgramStepBleKeyboardCodeRepeated<TLOGGERFAC, THIDBLEFAC>, false> {
  public:
    ProgramStepBleKeyboardCodeRepeated(KeyCode keyCode, uint8_t numRepetitions)
      : ProgramStepBase<TLOGGERFAC, ProgramStepBleKeyboardCodeRepeated<TLOGGERFAC, THIDBLEFAC>, false>(5 * numRepetitions), _keyCode(keyCode) {}

    void doTick(uint8_t tick) {
      if (tick % 5 == 0) {
        auto log = TLOGGERFAC::create();
        log->print(F("BK "));
        //ProgramStep<TLOGGER>::_log.println(_hexCode);
        auto ble = THIDBLEFAC::create();
        ble->sendKeyCode(_keyCode);
      }
    }

  private:
    KeyCode _keyCode;
};

template<class TLOGGERFAC, class THIDBLEFAC>
class ProgramStepBleControlKey : public ProgramStepBase<TLOGGERFAC, ProgramStepBleControlKey<TLOGGERFAC, THIDBLEFAC>, true> {
  public:
    ProgramStepBleControlKey(const char* ctrlKey)
      : ProgramStepBase<TLOGGERFAC, ProgramStepBleControlKey<TLOGGERFAC, THIDBLEFAC>, true>(1) {
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
        auto log = TLOGGERFAC::create();
        log->print(F("BC "));
        //ProgramStep<TLOGGER>::_log.println(_ctrlKey);
        auto ble = THIDBLEFAC::create();
        ble->print(F("AT+BleHidControlKey="));
        ble->println(_ctrlKey);

        if (ble->waitForOK()) {
          return;
        }
        log->println(F("Send keyboard text failed. Enable verbose mode for more information."));
      }
    }

  private:
    char* _ctrlKey;
};

template<class TLOGGERFAC, class THIDUSBFAC>
class ProgramStepUsbKeyboardText : public ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardText<TLOGGERFAC, THIDUSBFAC>, true> {
  public:
    ProgramStepUsbKeyboardText(const char* text)
      : ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardText<TLOGGERFAC, THIDUSBFAC>, true>(max(1, strlen(text) * 100 / TimerPeriod)) {
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
        auto log = TLOGGERFAC::create();
        log->print(F("UT "));
        auto usb = THIDUSBFAC::create();
        usb->print(_text);
      }
    }

  private:
    char* _text;
};

template<class TLOGGERFAC, class THIDUSBFAC>
class ProgramStepUsbKeyboardCode : public ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCode<TLOGGERFAC, THIDUSBFAC>, false> {
  public:
    ProgramStepUsbKeyboardCode(KeyCode keyCode)
      : ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCode<TLOGGERFAC, THIDUSBFAC>, false>(1), _keyCode(keyCode) {}

    void doTick(uint8_t tick) {
      if (tick == 0) {
        auto log = TLOGGERFAC::create();
        log->print(F("UK "));
        //ProgramStep<TLOGGER>::_log.println(_keyCode.ctrl);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.shift);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.alt);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.win);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.hexCode);
        auto usb = THIDUSBFAC::create();
        if (_keyCode.ctrl) {
          usb->press(0x80);
        }
        if (_keyCode.shift) {
          usb->press(0x81);
        }
        if (_keyCode.alt) {
          usb->press(0x82);
        }
        if (_keyCode.win) {
          usb->press(0x83);
        }
        usb->press(_keyCode.hexCode);
        usb->releaseAll();
      }
    }

  private:
    KeyCode _keyCode;
};

template<class TLOGGERFAC, class THIDUSBFAC>
class ProgramStepUsbKeyboardCodeRepeated : public ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCodeRepeated<TLOGGERFAC, THIDUSBFAC>, false> {
  public:
    ProgramStepUsbKeyboardCodeRepeated(KeyCode keyCode, uint8_t numRepetitions)
      : ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCodeRepeated<TLOGGERFAC, THIDUSBFAC>, false>(numRepetitions), _keyCode(keyCode) {}

    void doTick(uint8_t tick) {
      // _duration is equal to number of repetitions
      if (tick < ProgramStep<TLOGGERFAC>::_duration) {
        auto log = TLOGGERFAC::create();
        log->print(F("UK "));
        //ProgramStep<TLOGGER>::_log.println(_keyCode.ctrl);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.shift);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.alt);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.win);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.hexCode);
        auto usb = THIDUSBFAC::create();
        if (_keyCode.ctrl) {
          usb->press(0x80);
        }
        if (_keyCode.shift) {
          usb->press(0x81);
        }
        if (_keyCode.alt) {
          usb->press(0x82);
        }
        if (_keyCode.win) {
          usb->press(0x83);
        }
        usb->press(_keyCode.hexCode);
        usb->releaseAll();
      }
    }

  private:
    KeyCode _keyCode;
};

template<class TLOGGERFAC, class THIDUSBFAC>
class ProgramStepUsbKeyboardCodes : public ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCodes<TLOGGERFAC, THIDUSBFAC>, false> {
  public:
    ProgramStepUsbKeyboardCodes(KeyCode keyCode, char secondKey)
      : ProgramStepBase<TLOGGERFAC, ProgramStepUsbKeyboardCodes<TLOGGERFAC, THIDUSBFAC>, false>(1), _keyCode(keyCode), _secondKey(secondKey) {}

    void doTick(uint8_t tick) {
      // _duration is equal to number of repetitions
      if (tick < ProgramStep<TLOGGERFAC>::_duration) {
        auto log = TLOGGERFAC::create();
        log->print(F("UK "));
        //ProgramStep<TLOGGER>::_log.println(_keyCode.ctrl);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.shift);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.alt);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.win);
        //ProgramStep<TLOGGER>::_log.println(_keyCode.hexCode);
        auto usb = THIDUSBFAC::create();
        if (_keyCode.ctrl) {
          usb->press(0x80);
        }
        if (_keyCode.shift) {
          usb->press(0x81);
        }
        if (_keyCode.alt) {
          usb->press(0x82);
        }
        if (_keyCode.win) {
          usb->press(0x83);
        }
        usb->press(_keyCode.hexCode);
        usb->press(_secondKey);
        usb->releaseAll();
      }
    }

  private:
    KeyCode _keyCode;
    char _secondKey;
};
}

#endif
