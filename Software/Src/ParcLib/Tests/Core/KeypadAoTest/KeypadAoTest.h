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
  void print(T ch) {}
  template<class T>
  void print(T ch, uint8_t mode) {}
  template<class T>
  void println(T ch) {}
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

  Register registers;
  PinRegData pin;
  pin.code0 = 1;
  pin.code3 = 1;

  FakeKeypadHw keypadHw;
  KeypadAo<LoggerFac_t, FakeKeypadHw> keypad(&registers, keypadHw);

  registers.set(TERMINAL_KEYPAD_PIN, pin.raw);
  keypad.checkRegisters();

  EQ((RegisterData_t)0, registers.get(TERMINAL_KEYPAD_PIN));
  EQ((RegisterData_t)0, registers.get(KEYPAD_TERMINAL_PINALREADYDEFINED));
}

TEST(
  pin_received,
  pin_already_defined,
  pin_is_not_accepted_and_pin_already_defined_is_set) {

  Register registers;
  PinRegData pin;
  pin.code0 = 1;
  pin.code3 = 1;

  FakeKeypadHw keypadHw;
  KeypadAo<LoggerFac_t, FakeKeypadHw> keypad(&registers, keypadHw);

  // Set pin
  registers.set(TERMINAL_KEYPAD_PIN, pin.raw);
  keypad.checkRegisters();
  EQ((RegisterData_t)0, registers.get(TERMINAL_KEYPAD_PIN));
  EQ((RegisterData_t)0, registers.get(KEYPAD_TERMINAL_PINALREADYDEFINED));

  // Set pin again
  registers.set(TERMINAL_KEYPAD_PIN, pin.raw);
  keypad.checkRegisters();
  EQ((RegisterData_t)0, registers.get(TERMINAL_KEYPAD_PIN));
  EQ((RegisterData_t)1, registers.get(KEYPAD_TERMINAL_PINALREADYDEFINED));
}

TEST(
  key_pressed,
  pin_not_defined,
  hid_input_is_set) {

  Register registers;
  PinRegData pin;
  pin.code0 = 1;
  pin.code3 = 1;

  FakeKeypadHw keypadHw;
  KeypadAo<LoggerFac_t, FakeKeypadHw> keypad(&registers, keypadHw);

  keypadHw.buttonPressed = true;
  keypad.checkRegisters();

  KeypadRegData expected;
  expected.button = 5;
  EQ((RegisterData_t)expected.raw, registers.get(KEYPAD_HID_INPUT));
  EQ((RegisterData_t)0, registers.get(KEYPAD_TERMINAL_PINALREADYDEFINED));
}

END
}
