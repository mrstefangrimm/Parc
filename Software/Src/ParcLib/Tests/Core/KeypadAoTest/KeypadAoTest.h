// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/Shared.h"
#include "Core/KeypadAo.h"

namespace KeypadAoTest {

using namespace parclib;

struct FakeLogger {
  template<class T>
  void print(T) {}
  template<class T>
  void print(T, uint8_t) {}
  template<class T>
  void println(T) {}
};

struct FakeKeypadHw {

  bool buttonPressed = false;

  template<KeyPadSwitch SWITCH>
  void pinMode() {
  }

  template<KeyPadSwitch SWITCH>
  bool pressed() {
    return buttonPressed;
  }
};

template<>
bool FakeKeypadHw::pressed<Btn_A>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Btn_B>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Btn_C>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Btn_D>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Sw_M0>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Sw_M1>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Code_1>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Code_2>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Code_3>() {
  return false;
}

template<>
bool FakeKeypadHw::pressed<Code_4>() {
  return false;
}

}

KeypadAoTest::FakeLogger logger;
template<> KeypadAoTest::FakeLogger& parclib::Factory<KeypadAoTest::FakeLogger>::instance = logger;
using LoggerFac_t = parclib::Factory<KeypadAoTest::FakeLogger>;

namespace KeypadAoTest {

using namespace parclib;

BEGIN(KeypadAoTest)

TEST(
  pin_received,
  pin_not_yet_defined,
  pin_is_accepted) {

  Messages messages;
  PinRegData pin;
  pin.code0 = 1;
  pin.code3 = 1;

  FakeKeypadHw keypadHw;
  KeypadAo<LoggerFac_t, FakeKeypadHw> keypad(messages, keypadHw);

  messages.fromTerminalToKeypadQueue.push(pin.raw);
  keypad.load();
  keypad.run();

  EQ((MessageData_t)0, messages.fromTerminalToKeypadQueue.pop());
  TerminalData data(messages.toTerminalQueue.pop());
  FALSE(data.isPinDefined);
}

TEST(
  pin_received,
  pin_already_defined,
  pin_is_not_accepted_and_pin_already_defined_is_set) {

  Messages messages;
  PinRegData pin;
  pin.code0 = 1;
  pin.code3 = 1;

  FakeKeypadHw keypadHw;
  KeypadAo<LoggerFac_t, FakeKeypadHw> keypad(messages, keypadHw);

  // Set pin
  messages.fromTerminalToKeypadQueue.push(pin.raw);
  keypad.load();
  keypad.run();

  EQ((MessageData_t)0, messages.fromTerminalToKeypadQueue.pop());
  TerminalData data(messages.toTerminalQueue.pop());
  FALSE(data.isPinDefined);

  // Set pin again
  messages.fromTerminalToKeypadQueue.push(pin.raw);
  keypad.load();
  keypad.run();

  EQ((MessageData_t)0, messages.fromTerminalToKeypadQueue.pop());
  TerminalData data2(messages.toTerminalQueue.pop());
  TRUE(data2.isPinDefined);
}

TEST(
  key_pressed,
  pin_not_defined,
  hid_input_is_set) {

  Messages messages;
  PinRegData pin;
  pin.code0 = 1;
  pin.code3 = 1;

  FakeKeypadHw keypadHw;
  KeypadAo<LoggerFac_t, FakeKeypadHw> keypad(messages, keypadHw);

  keypadHw.buttonPressed = true;
  keypad.load();
  keypad.run();

  KeypadRegData expected;
  expected.button = 5;
  EQ((MessageData_t)expected.raw, messages.fromKeypadToHidQueue.pop());
  TerminalData data(messages.toTerminalQueue.pop());
  FALSE(data.isPinDefined);
}

END
}
