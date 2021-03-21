// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "StringParc.h"
#include "Shared.h"

namespace parclib {

  enum PsType {
    Wait,
    UsbKeycode,
    UsbKeycodeRepeated,
    UsbText,
    BleKeycode,
    BleKeycodeRepeated,
    BleText,
    BleControlkey
  };

  enum CmdType {
    _LastStep = BleControlkey,
    Pin
  };

  template<uint8_t CMDTYPE>
  struct CmdComparator {

    bool operator()(const char* another) const {
      return equals(another);
    }
    bool equals(const char* another) const { return false; }

    bool operator()(char** another) const {
      return equals(another);
    }
    bool equals(char** another) const { return false; }

  };


  template<class PROGSTEPFACTORY, class TSERIAL, class TLOGGER, class THIDBLE, class THIDUSB, uint8_t BUFLEN>
  class TerminalAo : public Ao<TerminalAo<PROGSTEPFACTORY, TSERIAL, TLOGGER, THIDBLE, THIDUSB, BUFLEN>> {
  public:
    TerminalAo(TSERIAL& serialInput, TLOGGER& logger, THIDBLE& ble, THIDUSB& usb, RegisterData_t* registers, Program<TLOGGER>* programs)
      : Ao_t(registers), _serial(serialInput), _state(State::Idle), _log(logger), _ble(ble), _usb(usb), _programs(programs) {}

    void checkRegisters() {
      if (Ao_t::_registers[TERMINAL_TERMINAL_TIMEOUT] != 0) {
        switch (_state) {
          case State::Idle: stateIdle(); break;
          case State::ReadingProgramCode: stateReadingProgramCode(); break;
          case State::ReadingProgramSteps: stateReadingProgramSteps(); break;
          case State::ReadingPin: stateReadingPin(); break;
        };

        Ao_t::_registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      }

      if (Ao_t::_registers[KEYPAD_TERMINAL_PINALREADYDEFINED] != 0) {
        _serial.println(F("PIN was not accepted. A PIN is already active."));
        Ao_t::_registers[KEYPAD_TERMINAL_PINALREADYDEFINED] = 0;
      }
    }

  private:
    void stateIdle() {
      char ch = 0;
      while (_serial.available() && _state == State::Idle) {
        ch = _serial.read();
        if (ch == '{') {
          // send blocked to KeypadAo
          _serial.print(F("> {"));

          _itBuf = 0;
          memset(_buf, 0, BUFLEN);
          ch = 0;
          _state = State::ReadingProgramCode;
        }
        else if (ch == '?') {
          _serial.println(F("Programmed:"));
          for (int m = 0; m < 4; m++) {
            for (int n = 1; n < 6; n++) {
              KeypadRegData input(m, n);
              _serial.print(input.mode); _serial.print(F(" ")); _serial.print(char(input.button - 1 + 'A')); _serial.print(F(" : "));
              _programs[input.programIndex()].hasSteps() ? _serial.println(F("Yes")) : _serial.println(F("No"));
            }
          }
         
        }
        else if (isprint(ch)) {
          _serial.print(ch);
        }        
      }
      if (ch != 0 /*&& ch != 0b1101 && ch != 0b1010*/) {
        // YAT sends 0b1101 and 0b1010, PuTTY sends 0b1101 and powershell sends 0b1010
        // Debug: Serial.println("Enter pressed");
        // Debug: Serial.println(); //eol of repeated command
        _serial.println(F("Start defining your dull programs."));
      }
    }

    void stateReadingProgramCode() {
      while (_serial.available() && _state == State::ReadingProgramCode) {
        char ch = _serial.read();
        _serial.print(ch);
        if (ch == ':') {
          parclib::trimFront(_buf, BUFLEN);
          parclib::trimBack(_buf, BUFLEN);
          parclib::squeeze(_buf);
          // Debug: _serial.println(_buf);
          uint8_t numSubStr;
          char* subStrs[2] = { 0 };
          parclib::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);          

          if (numSubStr == 2) {
            if (CmdComparator<CmdType::Pin>()(subStrs)) {
              _keyPadState.isPin = 1;
            }
            else {              
              // Debug: _serial.println(subStrs[0][0]); // Does not work when backspace is used in putty
              // Debug: _serial.println(subStrs[1][0]); // Does not work when backspace is used in putty
              // Debug: _serial.println(subStrs[0][strlen(subStrs[0])-1]);
              // Debug: _serial.println(subStrs[1][strlen(subStrs[1])-1]);
          
              _keyPadState.isPin = 0;
              _keyPadState.mode = subStrs[0][0] - '0';
              _keyPadState.button = subStrs[1][0] - 'A' + 1;

              // Debug: _serial.println(_keyPadState.mode);
              // Debug: _serial.println(_keyPadState.button);
            }
          }

          _itBuf = 0;
          memset(_buf, 0, BUFLEN);

          if (_keyPadState.isPin) {
            _log.println(F("got pin"));
            _state = State::ReadingPin;
          }
          else {
            auto idx = _keyPadState.programIndex();
            // Debug: _log.print(_keyPadState.mode); _log.print(F(" ")); _log.print(_keyPadState.button); _log.print(F(" > ")); _log.println(idx);
            if (0 <= idx && idx < NumberOfPrograms) {
              _state = State::ReadingProgramSteps;
              Program<TLOGGER>& prog = _programs[idx];
              if (prog.hasSteps()) {
                _log.println(F("got program index. Current program is being disposed."));
              }
              prog.dispose();
            }
            else {
              _serial.println(F(" This ain't dull, bye."));
              _state = State::Idle;
            }
          }    
        }
        else if (_itBuf < BUFLEN) {
          // omit preceding spaces to save the buffer for the data.
          if (_itBuf > 0 || ch != ' ') {
            _buf[_itBuf] = ch;
            _itBuf++;
          }
        }
      }
    }

    void stateReadingPin() {
      while (_serial.available() && _state == State::ReadingPin) {
        char ch = _serial.read();
        _serial.print(ch);
        if (ch == '}') {
          _serial.println(F(", thank you."));
          _state = State::Idle;
          // send unblocked to KeypadAo
        }
        else if (ch == ';') {
          // create programstep. If failed, go to wait state
          //_log.print(_buf);
          parclib::trimFront(_buf, BUFLEN);
          parclib::trimBack(_buf, BUFLEN);
          parclib::squeeze(_buf);
          uint8_t numSubStr;
          char* subStrs[6] = { 0 };
          parclib::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          if (numSubStr == 5) {
            PinRegData pin;
            pin.code3 = subStrs[0][0] == '1' ? 1 : 0;
            pin.code2 = subStrs[1][0] == '1' ? 1 : 0;
            pin.code1 = subStrs[2][0] == '1' ? 1 : 0;
            pin.code0 = subStrs[3][0] == '1' ? 1 : 0;
            pin.retries = atoi(subStrs[4]);

            Ao<TerminalAo<PROGSTEPFACTORY, TSERIAL, TLOGGER, THIDBLE, THIDUSB, BUFLEN>>::_registers[TERMINAL_KEYPAD_PIN] = pin.raw;
          }
          else {
            _serial.println(F(" This ain't dull, bye."));
            _state = State::Idle;
          }
          _itBuf = 0;
          memset(_buf, 0, BUFLEN);
        }
        else if (_itBuf < BUFLEN) {
          // omit preceding spaces to save the buffer for the data.
          if (_itBuf > 0 || ch != ' ') {
            _buf[_itBuf] = ch;
            _itBuf++;
          }
        }
      }
    }

    void stateReadingProgramSteps() {
      while (_serial.available() && _state == State::ReadingProgramSteps) {
        char ch = _serial.read();
        _serial.print(ch);
        if (ch == '}') {
          _serial.println(F(", thank you."));
          _state = State::Idle;
          // send unblocked to KeypadAo
        }
        else if (ch == ';') {
          // create programstep. If failed, go to wait state
          parclib::trimFront(_buf, BUFLEN);
          parclib::trimBack(_buf, BUFLEN);
          parclib::squeeze(_buf);
          // Debug: _serial.println(_buf);
          
          uint8_t numSubStr;
          char* subStrs[6] = { 0 };
          parclib::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          ProgramStep<TLOGGER>* progStep = 0;
         
          if (CmdComparator<PsType::Wait>()(subStrs[0])) {
            progStep = createProgramStepWait(subStrs[1]);
          }
          else if (CmdComparator<PsType::BleKeycode>()(subStrs[0])) {
            progStep = createProgramStepBleKeyboardCode(subStrs, numSubStr);
          }
          else if (CmdComparator<PsType::BleText>()(subStrs[0])) {
            progStep = createProgramStepBleKeyboardText(subStrs[1]);
          }
          else if (CmdComparator<PsType::BleControlkey>()(subStrs[0])) {
            progStep = createProgramStepBleControlKey(subStrs, numSubStr);
          }
          else if (CmdComparator<PsType::UsbKeycode>()(subStrs[0])) {
            progStep = createProgramStepUsbKeyboardCode(subStrs, numSubStr);
          }
          else if (CmdComparator<PsType::UsbText>()(subStrs[0])) {
            progStep = createProgramStepUsbKeyboardText(subStrs[1]);
          }

          if (progStep != 0) {
            uint8_t progIdx = _keyPadState.programIndex();
            _programs[progIdx].appendStep(progStep);
            Ao<TerminalAo<PROGSTEPFACTORY, TSERIAL, TLOGGER, THIDBLE, THIDUSB, BUFLEN>>::_registers[TERMINAL_MEMORY_CHANGE] = MemoryChangedRegData(1);
          }
          else {
            _serial.println(F(" Unknown command. This ain't dull, bye."));
            // Remove the already processed program steps from the program.
            uint8_t progIdx = _keyPadState.programIndex();
            _programs[progIdx].dispose();
            _state = State::Idle;
          }

          _itBuf = 0;
          memset(_buf, 0, BUFLEN);
        }
        else if (_itBuf < BUFLEN) {
          // omit preceding spaces to save the buffer for the data.
          if (_itBuf > 0 || ch != ' ') {
            _buf[_itBuf] = ch;
            _itBuf++;
          }
        }
        else {
          _serial.println(F(" Command string too long. This ain't dull, bye."));
          // Remove the already processed program steps from the program.
          uint8_t progIdx = _keyPadState.programIndex();
          _programs[progIdx].dispose();
          _state = State::Idle;
        }
      }
    }

    ProgramStep<TLOGGER>* createProgramStepWait(const char* delay) {
      uint16_t waitMs = atoi(delay);
      // Debug: _log.print(F("Delay: ")); _log.println(waitMs);
      
      _log.println(F("Create Wait_t"));
      return new Wait_t(_log, waitMs);
    }

    ProgramStep<TLOGGER>* createProgramStepBleKeyboardCode(char* subStrs[], uint8_t numSubStr) {
      KeyCode keyCode;
      uint8_t repetitions = 0;
      for (int n = 1; n < numSubStr; n++) {
        keyCode.ctrl |= strcmp(subStrs[n], "<Ctrl>") == 0;
        keyCode.shift |= strcmp(subStrs[n], "<Shift>") == 0;
        keyCode.alt |= strcmp(subStrs[n], "<Alt>") == 0;
        keyCode.win |= strcmp(subStrs[n], "<Win>") == 0;

        if (strcmp2("-r", subStrs[n][0], subStrs[n][1])) {
          repetitions = atoi(&subStrs[n][2]);
        }
      }

      auto code = subStrs[numSubStr - 1];
      keyCode.hexCode = strtol(subStrs[numSubStr - 1], 0, BleKeycode_t::Radix);
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Del>") == 0 ? BleKeycode_t::KeyCodeDel : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Tab>") == 0 ? BleKeycode_t::KeyCodeTab : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Enter>") == 0 ? BleKeycode_t::KeyCodeEnter : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Space>") == 0 ? BleKeycode_t::KeyCodeSpace : keyCode.hexCode : keyCode.hexCode;

      //_log.print("ProgramStepBleKeyboardCode "); _log.print(hexCode, HEX);
      if (repetitions == 0) {
        _log.println(F("Create BleKeycode_t"));
        return new BleKeycode_t(_log, _ble, keyCode);
      }
      _log.println(F("Create BleKeycodeRepeated_t"));
      return new BleKeycodeRepeated_t(_log, _ble, keyCode, repetitions);
    }

    ProgramStep<TLOGGER>* createProgramStepBleKeyboardText(char* text) {
      //_log.print("TEXT: "); _log.println(text);
      _log.println(F("Create BleText_t"));
      return new BleText_t(_log, _ble, text);
    }

    ProgramStep<TLOGGER>* createProgramStepBleControlKey(char* subStrs[], uint8_t numSubStr) {
      auto duration = numSubStr > 2 ? atoi(subStrs[2]) : 1;
      // _log.print("Control Key: "); _log.print(subStrs[1]); _log.print(" for: "); _log.println(duration);   

      if (strcmp(subStrs[1], "Volume+") == 0) { return new BleControlkey_t(_log, _ble, "0xE9", duration); }
      if (strcmp(subStrs[1], "Volume-") == 0) { return new BleControlkey_t(_log, _ble, "0xEA", duration); }
      _log.println(F("Create BleControlkey_t"));
      return new BleControlkey_t(_log, _ble, subStrs[1], duration);
    }

    ProgramStep<TLOGGER>* createProgramStepUsbKeyboardCode(char* subStrs[], uint8_t numSubStr) {
      KeyCode keyCode;
      uint8_t repetitions = 0;
      for (int n = 1; n < numSubStr; n++) {
        keyCode.ctrl |= strcmp(subStrs[n], "<Ctrl>") == 0;
        keyCode.shift |= strcmp(subStrs[n], "<Shift>") == 0;
        keyCode.alt |= strcmp(subStrs[n], "<Alt>") == 0;
        keyCode.win |= strcmp(subStrs[n], "<Win>") == 0;

        if (strcmp2("-r", subStrs[n][0], subStrs[n][1])) {
          repetitions = atoi(&subStrs[n][2]);
        }
      }

      auto code = subStrs[numSubStr - 1];
      //Debug: _log.print("createProgramStepUsbKeyboardCode "); _log.println(code);
      if (strlen(code) == 3 && code[0] == '\'' && code[2] == '\'') {
        keyCode.hexCode = code[1];
      }
      else {
        keyCode.hexCode = strtol(code, 0, UsbKeycode_t::Radix);
      }
      //Debug: _log.print("createProgramStepUsbKeyboardCode "); _log.println(keyCode.hexCode, HEX);

      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Del>") == 0 ? UsbKeycode_t::KeyCodeDel : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Tab>") == 0 ? UsbKeycode_t::KeyCodeTab : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Enter>") == 0 ? UsbKeycode_t::KeyCodeEnter : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(code, "<Space>") == 0 ? UsbKeycode_t::KeyCodeSpace : keyCode.hexCode : keyCode.hexCode;

      //Debug: _log.print("createProgramStepUsbKeyboardCode "); _log.println(keyCode.hexCode, HEX);
      if (repetitions == 0) {
        _log.println(F("Create UsbKeycode_t"));
        return new UsbKeycode_t(_log, _usb, keyCode);
      }
      _log.println(F("Create UsbKeycodeRepeated_t"));
      return new UsbKeycodeRepeated_t(_log, _usb, keyCode, repetitions);
    }

    ProgramStep<TLOGGER>* createProgramStepUsbKeyboardText(char* text) {
      // _log.print("TEXT: "); _log.println(text);
      _log.println(F("Create UsbText_t"));
      return new UsbText_t(_log, _usb, text);
    }
    
  private:
    typedef Ao<TerminalAo<PROGSTEPFACTORY, TSERIAL, TLOGGER, THIDBLE, THIDUSB, BUFLEN>> Ao_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::Wait>::Result Wait_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::BleKeycode>::Result BleKeycode_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::BleKeycodeRepeated>::Result BleKeycodeRepeated_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::BleText>::Result BleText_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::BleControlkey>::Result BleControlkey_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::UsbKeycode>::Result UsbKeycode_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::UsbKeycodeRepeated>::Result UsbKeycodeRepeated_t;
    typedef typename TypeAt<PROGSTEPFACTORY, PsType::UsbText>::Result UsbText_t;

    enum class State {
      Blocked,
      Idle,
      ReadingProgramCode,
      ReadingProgramSteps,
      ReadingPin
    };
    State _state;
    TSERIAL& _serial;
    TLOGGER& _log;
    THIDBLE& _ble;
    THIDUSB& _usb;
    Program<TLOGGER>* _programs;

    char _buf[BUFLEN] = { 0 };
    uint8_t _itBuf = 0;

    KeypadRegData _keyPadState;

  };

}
