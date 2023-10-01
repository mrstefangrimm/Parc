// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Core/Shared.h"
#include "Core/Program.h"
#include "Feather/ProgramSteps.h"

using namespace parclib;

namespace ProgramTest {

struct FakeLogger {
  template<class T>
  void print(T) {}
  template<class T>
  void print(T, uint8_t) {}
  template<class T>
  void println(T) {}
};

struct FakeUsbKeyboard {

  int printInvokeCounter = 0;

  void print(const char*) {
    printInvokeCounter++;
  }
  void press(uint8_t) {}
  void releaseAll() {}

private:
  void println(const char*) {}
};

struct FakeHidBle {

  bool sendKeyCode(KeyCode) {
    return false;
  }
  bool waitForOK() {
    return true;
  }

  void print(const char*) {}
  void print(const __FlashStringHelper*) {}

  int printlnInvokeCounter = 0;

  void println(const char* text) {
    strcpy(lastPrintln, text);
    printlnInvokeCounter++;
  }

  char lastPrintln[30];
};
}

ProgramTest::FakeLogger logger;
template<> ProgramTest::FakeLogger& Factory<ProgramTest::FakeLogger>::instance = logger;
typedef Factory<ProgramTest::FakeLogger> LoggerFac_t;

ProgramTest::FakeUsbKeyboard usb;
template<> ProgramTest::FakeUsbKeyboard& Factory<ProgramTest::FakeUsbKeyboard>::instance = usb;
typedef Factory<ProgramTest::FakeUsbKeyboard> HidUsbFac_t;

ProgramTest::FakeHidBle ble;
template<> ProgramTest::FakeHidBle& Factory<ProgramTest::FakeHidBle>::instance = ble;
typedef Factory<ProgramTest::FakeHidBle> HidBleFac_t;

namespace ProgramTest {

// Test naming scheme: Given-When-Then

BEGIN(ProgramTest)

INIT(Initialize, { reset(); })

TEST(
  w,
  5000ms,
  50ticks) {
  auto p = new ProgramStepWait<LoggerFac_t>(5000);

  EQ((size_t)50, p->duration());

  p->dispose();
  delete p;
}

TEST(
  bt,
  hello,
  duration_at_least_10ticks) {
  auto p = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello");

  EQ((size_t)10, p->duration());

  p->dispose();
  delete p;
}

TEST(
  bt,
  hello_world,
  duration_strlen_11ticks) {
  auto p = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello World");

  EQ((size_t)11, p->duration());

  p->dispose();
  delete p;
}

TEST(
  bt,
  hello_world_played,
  print_is_invoked) {
  auto p = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello World");

  uint8_t tick = 0;
  p->play(tick);
  EQ(1, ble.printlnInvokeCounter);

  p->dispose();
  delete p;
}

TEST(
  bk,
  any,
  duration_5ticks) {
  auto p = new ProgramStepBleKeyboardCode<LoggerFac_t, HidBleFac_t>(0x0F);

  EQ((size_t)5, p->duration());

  p->dispose();
  delete p;
}

TEST(
  bk_repeated,
  any_4_times,
  duration_20ticks) {
  auto p = new ProgramStepBleKeyboardCodeRepeated<LoggerFac_t, HidBleFac_t>(0x0F, 4);

  EQ((size_t)20, p->duration());

  p->dispose();
  delete p;
}

TEST(
  bc,
  mute,
  duration_1tick) {
  auto p = new ProgramStepBleControlKey<LoggerFac_t, HidBleFac_t>("Mute");

  EQ((size_t)1, p->duration());

  p->dispose();
  delete p;
}

TEST(
  bc,
  volume,
  duration_1tick) {
  auto p = new ProgramStepBleControlKey<LoggerFac_t, HidBleFac_t>("Volume+");

  EQ((size_t)1, p->duration());

  p->dispose();
  delete p;
}

TEST(
  ut,
  empty,
  duration_at_least_1ticks) {
  auto p = new ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>("");

  EQ((size_t)1, p->duration());

  p->dispose();
  delete p;
}

TEST(
  ut,
  hello_world,
  duration_strlen_11ticks) {
  auto p = new ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>("Hello World");

  EQ((size_t)11, p->duration());

  p->dispose();
  delete p;
}

TEST(
  ut,
  hello_world_played,
  print_is_invoked) {
  auto p = new ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>("Hello World");

  uint8_t tick = 0;
  p->play(tick);
  EQ(1, usb.printInvokeCounter);

  p->dispose();
  delete p;
}

TEST(
  uk,
  any,
  duration_1ticks) {
  auto p = new ProgramStepUsbKeyboardCode<LoggerFac_t, HidUsbFac_t>(0x0F);

  EQ((size_t)1, p->duration());

  p->dispose();
  delete p;
}

TEST(
  uk_repeated,
  any_4_times,
  duration_4ticks) {
  auto p = new ProgramStepUsbKeyboardCodeRepeated<LoggerFac_t, HidUsbFac_t>(0x0F, 4);

  EQ((size_t)4, p->duration());

  p->dispose();
  delete p;
}

TEST(
  uk_codes,
  any,
  duration_1ticks) {
  auto p = new ProgramStepUsbKeyboardCodes<LoggerFac_t, HidUsbFac_t>('k', 'd');

  EQ((size_t)1, p->duration());

  p->dispose();
  delete p;
}

TEST(
  valid_three_program_steps,
  through,
  hello_is_sent_last) {
  auto p1 = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello");
  auto p2 = new ProgramStepWait<LoggerFac_t>(200);
  auto p3 = new ProgramStepBleKeyboardCode<LoggerFac_t, HidBleFac_t>(0x1);
  p1->appendStep(p2);
  p2->appendStep(p3);

  Program<LoggerFac_t> p;
  p.appendStep(p1);

  p.play();  // 0 BT Hello
  p.play();  // 1
  p.play();  // 2
  p.play();  // 3
  p.play();  // 4
  p.play();  // 5
  p.play();  // 6
  p.play();  // 7
  p.play();  // 8
  p.play();  // 9
  p.play();  // 10 => 0 W 200
  p.play();  // 1
  p.play();  // 2 => 0 BK 0x1
  p.play();  // 1
  p.play();  // 2
  p.play();  // 3
  p.play();  // 4
  p.play();  // 5 => 0 rewind is required
  p.play();  // 5 => 0
  p.play();  // 5 => 0
  p.rewind();
  p.play();  // 0 BT Hello

  EQ(0, strcmp("Hello", ble.lastPrintln));

  p.dispose();
}

void reset() {
  HidUsbFac_t::instance.printInvokeCounter = 0;
  HidBleFac_t::instance.printlnInvokeCounter = 0;
}

END

}
