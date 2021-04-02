// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <list>

#define F(x) (x)
#define max(a,b) ((a)>(b)?(a):(b))
enum SerialFakeMode { BIN, HEX };
const int INPUT_PULLUP = 1;
const int OUTPUT = 2;

#include "Domain/HidAo.h"
#include "Domain/Shared.h"
#include "Domain/Program.h"
#include "Domain/KeypadAo.h"
#include "Domain/TerminalAo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace parclib;
using namespace std;

template<> bool CmdComparator<PsType::UsbKeycode>::equals(const char* another) const { return 'U' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::Wait>::equals(const char* another) const { return 'W' == another[0]; }
template<> bool CmdComparator<PsType::UsbText>::equals(const char* another) const { return 'U' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleKeycode>::equals(const char* another) const { return 'B' == another[0] && 'K' == another[1]; }
template<> bool CmdComparator<PsType::BleText>::equals(const char* another) const { return 'B' == another[0] && 'T' == another[1]; }
template<> bool CmdComparator<PsType::BleControlkey>::equals(const char* another) const { return 'B' == another[0] && 'C' == another[1]; }
template<> bool CmdComparator<CmdType::Pin>::equals(char** another) const { return 'P' == another[0][0] && 'N' == another[1][0]; }

namespace TerminalAoTest
{
  struct FakeLogger {
    template<class T>
    void print(T ch) { }
    template<class T>
    void print(T ch, uint8_t mode) {}
    template<class T>
    void println(T ch) {}
  } logger;

  struct FakeKeypadHw {

    template<KeyPadSwitch SWITCH>
    void pinMode() {
    }

    template<KeyPadSwitch SWITCH>
    bool pressed() {
      return false;
    }

  } keypadHwFake;

  class FakeHidBle {
  public:
    bool sendKeyCode(KeyCode keyCode) {
      return false;
    }
    bool waitForOK() {
      return true;
    }

    void print(const char*) {}
    void println(const char*) {}
  } ble;

  class FakeSerial {
  public:
    void setInputBuffer(const char* terminalInput) {
      for (size_t n = 0; n < strlen(terminalInput); n++) {
        _buf.push_back(terminalInput[n]);
      }
    }

    template<class T>
    void print(T ch) { cout << ch; }
    template<class T>
    void print(T ch, SerialFakeMode mode) { cout << ch; }
    template<class T>
    void println(T ch) { cout << ch << endl; }
    template<class T>
    void println(T ch, SerialFakeMode mode) { cout << ch << endl; }
    void println() { cout << endl; }

    bool available() { return _buf.size() > 0; }
    int read() {
      if (_buf.size() > 0) {
        char ret = _buf.front();
        _buf.pop_front();
        return ret;
      }
      return -1;
    }

  private:
    list<char> _buf;
  } Serial;

  class FakeKeyboard {
  public:
    size_t press(uint8_t k) { return 1; }
    void releaseAll() {

    }
    void print(const char*) {}
    void println(const char*) {}
  } usb;

  template<class TLOGGER>
  struct FakeProgram {
    ProgramStep<TLOGGER>* appendStep(ProgramStep<TLOGGER>* step) {
      if (root == 0) { root = step; currentStep = step; return root; }
      else { return root->appendStep(step); }
    }

    void dispose() {
      if (root != 0) {
        root->dispose();
        delete root;
        root = 0;
      }
    }
    
    bool hasSteps() const {
      return root != 0;
    }

    ProgramStep<TLOGGER>* root = 0;
    ProgramStep<TLOGGER>* currentStep = 0;
  };

  FakeProgram<FakeLogger> programs[NumberOfPrograms];
  RegisterData_t registers[TOTAL_REGISTERS] = { 0 };

  struct KnownKeycodes {
    static const uint8_t UsbRadix = 16;
    static const uint8_t UsbKeyCodeDel = 0xD4;
    static const uint8_t UsbKeyCodeTab = 0xB3;
    static const uint8_t UsbKeyCodeEnter = 0xB0;
    static const uint8_t UsbKeyCodeSpace = ' ';

    static const uint8_t BleRadix = 16;
    static const uint8_t BleKeyCodeDel = 0x4C;
    static const uint8_t BleKeyCodeTab = 0x2B;
    static const uint8_t BleKeyCodeEnter = 0x28;
    static const uint8_t BleKeyCodeSpace = 0x2C;
  };

  struct FakeProgramStep : public ProgramStep<FakeLogger> {
    FakeProgramStep(FakeLogger& logger, uint16_t waitMs) : ProgramStep<FakeLogger>(logger, 0) { type = PsType::Wait; }
    FakeProgramStep(FakeLogger& logger, FakeKeyboard& usb, const char* text) : ProgramStep<FakeLogger>(logger, 0) { type = PsType::UsbText; }
    FakeProgramStep(FakeLogger& logger, FakeKeyboard& usb, KeyCode keyCode) : ProgramStep(logger, 0) { type = PsType::UsbKeycode; hexCode = keyCode.hexCode; }
    FakeProgramStep(FakeLogger& logger, FakeKeyboard& usb, KeyCode keyCode, uint8_t repetitions) : ProgramStep(logger, 0) { type = PsType::UsbKeycodeRepeated; hexCode = keyCode.hexCode; }
    FakeProgramStep(FakeLogger& logger, FakeKeyboard& usb, KeyCode keyCode, char secondKey) : ProgramStep<FakeLogger>(logger, 0) { type = PsType::UsbKeycodes; hexCode = keyCode.hexCode; }
    FakeProgramStep(FakeLogger& logger, FakeHidBle& ble, const char* text) : ProgramStep<FakeLogger>(logger, 0) { type = strcmp("Mute", text) == 0 ?  type = PsType::BleControlkey : PsType::BleText; }
    FakeProgramStep(FakeLogger& logger, FakeHidBle& ble, KeyCode keyCode) : ProgramStep(logger, 0) { type = PsType::BleKeycode; hexCode = keyCode.hexCode; }
    FakeProgramStep(FakeLogger& logger, FakeHidBle& ble, KeyCode keyCode, uint8_t repetitions) : ProgramStep(logger, 0) { type = PsType::BleKeycodeRepeated; hexCode = keyCode.hexCode; }

    void action(VirtualAction type, uint8_t& tick) override {};

    PsType type;
    uint8_t hexCode = 0;
  };

  KeypadAo<FakeLogger, FakeKeypadHw> keypadAo(registers, logger, keypadHwFake);
  HidAo<FakeLogger, FakeProgram<FakeLogger>> hid(logger, registers, programs);

  // Has to filled in the order of the enum PsType, that is:
  //  Wait, USB Keycode, USB Keycode Repeated, USB Keycodes, USB Text, BLE Keycode, BLE Keycode Repeated, BLE Text, BLE Control Key
  typedef Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    Typelist<FakeProgramStep,
    NullType>>>>>>>>> ProgramStepList;

  TerminalAo<ProgramStepList, FakeSerial, FakeLogger, FakeHidBle, FakeKeyboard, FakeProgram<FakeLogger>, KnownKeycodes, 40> terminal(Serial, logger, ble, usb, registers, programs);


  // Test naming scheme: Given-When-Then

  TEST_CLASS(TerminalAoTest)
  {
  public:

    TEST_METHOD(given_valid_w_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: W 1000; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::Wait, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_uk_ctrl_k_d_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK <Ctrl> \"k d\"; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycodes, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_uk_l_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK 'l'; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_uk_win_l_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK <Win> 'l'; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_uk_ctrl_alt_del_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK <Ctrl> <Alt> <Del>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::UsbKeyCodeDel, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_uk_enter_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK <Enter>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::UsbKeyCodeEnter, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_uk_win_space_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK <Win> <Space>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::UsbKeyCodeSpace, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_uk_tab_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK <Tab>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::UsbKeyCodeTab, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_uk_0xFF_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK 0xFF; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(0xFF, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_uk_r4_10_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UK -r4 10; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbKeycodeRepeated, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(16, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_bk_l_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK 'l'; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_bk_win_l_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK <Win> 'l'; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_bk_ctrl_alt_del_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK <Ctrl> <Alt> <Del>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::BleKeyCodeDel, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_bk_enter_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK <Enter>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::BleKeyCodeEnter, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_bk_win_space_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK <Win> <Space>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::BleKeyCodeSpace, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_bk_tab_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK <Tab>; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(KnownKeycodes::BleKeyCodeTab, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_bk_0xFF_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK 0xFF; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycode, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(0xFF, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }

    TEST_METHOD(given_valid_bk_r4_10_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BK -r4 10; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleKeycodeRepeated, static_cast<FakeProgramStep*>(programs[0].root)->type);
      Assert::AreEqual<uint8_t>(16, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
    }


    TEST_METHOD(given_valid_ut_one_word_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UT hello; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbText, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_ut_two_words_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: UT \"hello parc\"; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::UsbText, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_bt_one_word_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BT hello; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleText, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_bt_two_words_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BT \"hello parc\"; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleText, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_bc_mute_when_read_then_added_step) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ 0 A: BC Mute; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }
      Assert::IsNotNull(programs[0].root);
      Assert::AreEqual<uint8_t>(PsType::BleControlkey, static_cast<FakeProgramStep*>(programs[0].root)->type);
    }

    TEST_METHOD(given_valid_pin_when_read_then_set) {

      registers[TERMINAL_TERMINAL_TIMEOUT] = TimerRegData(1);
      Serial.setInputBuffer("{ P N: 1 0 1 1 3; }");

      for (int n = 0; n < 100; n++) {
        terminal.checkRegisters();
      }

      PinRegData regData = registers[TERMINAL_KEYPAD_PIN];
      Assert::AreEqual<uint8_t>(regData.code3, 1);
      Assert::AreEqual<uint8_t>(regData.code2, 0);
      Assert::AreEqual<uint8_t>(regData.code1, 1);
      Assert::AreEqual<uint8_t>(regData.code0, 1);
      Assert::AreEqual<uint8_t>(regData.retries, 3);
    }

  };

}
