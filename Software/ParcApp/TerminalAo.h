// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "StringAlgo.h"

namespace parc {

  template<typename TSERIAL, typename TLOGGER, typename THIDBLE, typename THIDUSB, uint8_t BUFLEN>
  class TerminalAo : public Ao {
  public:
    TerminalAo(TSERIAL& serialInput, TLOGGER& logger, THIDBLE& ble, THIDUSB& usb, RegisterData_t* registers, Program<TLOGGER>* programs)
      : Ao(registers), _serial(serialInput), _state(State::Idle), _log(logger), _ble(ble), _usb(usb), _programs(programs) {}

    void checkRegisters() {
      if (_registers[TERMINAL_TERMINAL_TIMEOUT] != 0) {
        switch (_state) {
          case State::Idle: stateIdle(); break;
          case State::ReadingProgramCode: stateReadingProgramCode(); break;
          case State::ReadingProgramSteps: stateReadingProgramSteps(); break;
          case State::ReadingPin: stateReadingPin(); break;
        };

        _registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
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
          _serial.println(F("Programs defined:"));
          for (int n = 0; n < NumberOfPrograms; n++) {
            KeypadRegData input(n);
            _serial.print(input.profile); _serial.print(" "); _serial.print(char(input.button + 'A')); _serial.print(" : ");
            _programs[input.programIndex()].steps() == 0 ? _serial.println(F("No")) : _serial.println(F("Yes"));
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
        _serial.println("Start defining your dull programs.");
      }
    }

    void stateReadingProgramCode() {
      while (_serial.available() && _state == State::ReadingProgramCode) {
        char ch = _serial.read();
        _serial.print(ch);
        if (ch == ':') {
          parc::trimFront(_buf, BUFLEN);
          parc::trimBack(_buf, BUFLEN);
          uint8_t numSubStr;
          char* subStrs[2] = { 0 };
          parc::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          if (numSubStr == 2) {
            if (subStrs[0][0] == 'P' && subStrs[1][0] == 'N') {
              _keyPadState.isPin = 1;
            }
            else {
              _keyPadState.isPin = 0;
              _keyPadState.profile = subStrs[0][0] - '0';
              _keyPadState.button = subStrs[1][0] - 'A' + 1;
            }
          }

          _itBuf = 0;
          memset(_buf, 0, BUFLEN);

          if (_keyPadState.isPin) {
            _log.print(F("got pin"));
            _state = State::ReadingPin;
          }
          else {
            auto idx = _keyPadState.programIndex();
            _log.print(_keyPadState.profile); _log.print(" "); _log.print(_keyPadState.button);_log.print(" > "); _log.print(idx);
            if (0 <= idx && idx < NumberOfPrograms) {
              //_log.print(_keyPadState.profile); _log.print(F(" ")); _log.print(_keyPadState.button); _log.print(F(": "));
              _state = State::ReadingProgramSteps;
              _programs[idx].dispose();
            }
            else {
              _serial.println(F(" This ain't dull, by."));
              _state = State::Idle;
            }
          }

         /* if (_keyPadState.raw == 0) {
            _serial.println(F(" This ain't dull, bye."));
            _state = State::Idle;
          }
          else {
           
          }   */      
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
          parc::trimFront(_buf, BUFLEN);
          parc::trimBack(_buf, BUFLEN);
          uint8_t numSubStr;
          char* subStrs[6] = { 0 };
          parc::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          if (numSubStr == 5) {
            PinRegData pin;
            pin.code3 = subStrs[0][0] == '1' ? 1 : 0;
            pin.code2 = subStrs[1][0] == '1' ? 1 : 0;
            pin.code1 = subStrs[2][0] == '1' ? 1 : 0;
            pin.code0 = subStrs[3][0] == '1' ? 1 : 0;
            pin.retries = atoi(subStrs[4]);

            _registers[TERMINAL_KEYPAD_PIN] = pin.raw;
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
          //_log.print(_buf);
          parc::trimFront(_buf, BUFLEN);
          parc::trimBack(_buf, BUFLEN);
          uint8_t numSubStr;
          char* subStrs[6] = { 0 };
          parc::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          ProgramStep<TLOGGER>* progStep = 0;
          if (subStrs[0][0] == 'W') {
            progStep = createProgramStepWait(subStrs[1]);
          }
          else if (parc::strcmp2(subStrs[0], 'B', 'K')) {
            progStep = createProgramStepBleKeyboardCode(subStrs, numSubStr);
          }
          else if (parc::strcmp2(subStrs[0], 'B', 'T')) {
            progStep = createProgramStepBleKeyboardText(subStrs[1]);
          }
          else if (parc::strcmp2(subStrs[0], 'B', 'C')) {
            progStep = createProgramStepBleControlKey(subStrs, numSubStr);
          }
          else if (parc::strcmp2(subStrs[0], 'U', 'K')) {
            progStep = createProgramStepUsbKeyboardCode(subStrs, numSubStr);
          }
          else if (parc::strcmp2(subStrs[0], 'U', 'T')) {
            progStep = createProgramStepUsbKeyboardText(subStrs[1]);
          }

          if (progStep != 0) {
            uint8_t progIdx = _keyPadState.programIndex();
            _programs[progIdx].appendStep(progStep);
            _registers[TERMINAL_MEMORY_CHANGE] = MemoryChangedRegData(1);
          }
          //else {
          //  _serial.println(F(" This ain't dull, bye bye."));
          //  _state = State::Idle;
          //}

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

    ProgramStep<TLOGGER>* createProgramStepWait(const char* delay) {
      uint16_t waitMs = atoi(delay);
      //_log.print("DELAY: "); _log.println(waitMs);
      return new ProgramStepWait<TLOGGER>(_log, waitMs);
    }

    ProgramStep<TLOGGER>* createProgramStepBleKeyboardCode(char* subStrs[], uint8_t numSubStr) {
      KeyCode keyCode;
      for (int n = 1; n < numSubStr; n++) {
        keyCode.ctrl |= strcmp(subStrs[n], "<Ctrl>") == 0;
        keyCode.shift |= strcmp(subStrs[n], "<Shift>") == 0;
        keyCode.alt |= strcmp(subStrs[n], "<Alt>") == 0;
        keyCode.win |= strcmp(subStrs[n], "<Win>") == 0;
      }

      keyCode.hexCode = strtol(subStrs[numSubStr - 1], 0, 16);
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Del>") == 0 ? 0x4C : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Enter>") == 0 ? 0x28 : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Tab>") == 0 ? 0x2B : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Space>") == 0 ? 0x2C : keyCode.hexCode : keyCode.hexCode;

      //_log.print("ProgramStepBleKeyboardCode "); _log.print(hexCode, HEX);
      return new ProgramStepBleKeyboardCode<TLOGGER, THIDBLE>(_log, _ble, keyCode);
    }

    ProgramStep<TLOGGER>* createProgramStepBleKeyboardText(char* text) {
      //_log.print("TEXT: "); _log.println(text);
      return new ProgramStepBleKeyboardText<TLOGGER, THIDBLE>(_log, _ble, text);
    }

    ProgramStep<TLOGGER>* createProgramStepBleControlKey(char* subStrs[], uint8_t numSubStr) {
      auto duration = numSubStr > 2 ? atoi(subStrs[2]) : 1;
      // _log.print("Control Key: "); _log.print(subStrs[1]); _log.print(" for: "); _log.println(duration);   

      if (strcmp(subStrs[1], "Volume+") == 0) { return new ProgramStepBleControlKey<TLOGGER, THIDBLE>(_log, _ble, "0xE9", duration); }
      if (strcmp(subStrs[1], "Volume-") == 0) { return new ProgramStepBleControlKey<TLOGGER, THIDBLE>(_log, _ble, "0xEA", duration); }
      return new ProgramStepBleControlKey<TLOGGER, THIDBLE>(_log, _ble, subStrs[1], duration);
    }

    ProgramStep<TLOGGER>* createProgramStepUsbKeyboardCode(char* subStrs[], uint8_t numSubStr) {
      KeyCode keyCode;
      for (int n = 1; n < numSubStr; n++) {
        keyCode.ctrl |= strcmp(subStrs[n], "<Ctrl>") == 0;
        keyCode.shift |= strcmp(subStrs[n], "<Shift>") == 0;
        keyCode.alt |= strcmp(subStrs[n], "<Alt>") == 0;
        keyCode.win |= strcmp(subStrs[n], "<Win>") == 0;
      }

      keyCode.hexCode = strtol(subStrs[numSubStr - 1], 0, 16); // 0x0;
      auto code = subStrs[numSubStr - 1];
      if (strlen(code) == 3 && code[0] == '\'' && code[2] == '\'') {
        keyCode.hexCode = code[1];
      }
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Del>") == 0 ? 0xD4 : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Enter>") == 0 ? 0xB0 : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Tab>") == 0 ? 0xB3 : keyCode.hexCode : keyCode.hexCode;
      keyCode.hexCode = keyCode.hexCode == 0 ? strcmp(subStrs[numSubStr - 1], "<Space>") == 0 ? ' ' : keyCode.hexCode : keyCode.hexCode;

      // _log.print("createProgramStepUsbKeyboardCode "); _log.print(hexCode, HEX);
      return new ProgramStepUsbKeyboardCode<TLOGGER, THIDUSB>(_log, _usb, keyCode);
    }

    ProgramStep<TLOGGER>* createProgramStepUsbKeyboardText(char* text) {
      // _log.print("TEXT: "); _log.println(text);
      return new ProgramStepUsbKeyboardText<TLOGGER, THIDUSB>(_log, _usb, text);
    }
    
  private:
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
