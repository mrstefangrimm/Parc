// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>

#include "Domain/Shared.h"
#include "Domain/Program.h"
#include "Feather/ProgramSteps.h"

using namespace parclib;

struct FakeHidBle {
  bool sendKeyCode(KeyCode keyCode) {
    return false;
  }
  bool waitForOK() {
    return true;
  }
  void print(const __FlashStringHelper*) {}
  void print(const char*) {}
  void println(const char* text) {
    strcpy(lastPrintln, text);
  }

  char lastPrintln[30];
} ble;

struct FakeKeyboard {
  size_t press(uint8_t k) { return 1; }
  void releaseAll() {

  }
  void print(const char*) {}
  void println(const char*) {}
} usb;

struct FakeLogger {
  template<class T>
  void print(T ch) { }
  template<class T>
  void print(T ch, uint8_t mode) {}
  template<class T>
  void println(T ch) {}
} logger;


// Test naming scheme: Given-When-Then

test(given_w_when_5000ms_then_50ticks) {
  auto p = new ProgramStepWait<FakeLogger>(logger, 5000);

  assertEqual(50, p->duration());

  p->dispose();
  delete p;
}

test(given_bt_when_hello_then_duration_at_least_10ticks) {
  auto p = new ProgramStepBleKeyboardText<FakeLogger, FakeHidBle>(logger, ble, "Hello");
      
  assertEqual(10, p->duration());

  p->dispose();
  delete p;
}

test(given_bt_when_hello_world_then_duration_strlen_11ticks) {
  auto p = new ProgramStepBleKeyboardText<FakeLogger, FakeHidBle>(logger, ble, "Hello World");

  assertEqual(11, p->duration());

  p->dispose();
  delete p;
}

test(given_bk_when_any_then_duration_5ticks) {
  auto p = new ProgramStepBleKeyboardCode<FakeLogger, FakeHidBle>(logger, ble, 0x0F);

  assertEqual(5, p->duration());

  p->dispose();
  delete p;
}

test(given_bk_repeated_when_any_4_times_then_duration_20ticks) {
  auto p = new ProgramStepBleKeyboardCodeRepeated<FakeLogger, FakeHidBle>(logger, ble, 0x0F, 4);

  assertEqual(20, p->duration());

  p->dispose();
  delete p;
}

test(given_bc_when_mute_then_duration_1tick) {
  auto p = new ProgramStepBleControlKey<FakeLogger, FakeHidBle>(logger, ble, "Mute");

  assertEqual(1, p->duration());

  p->dispose();
  delete p;
}

test(given_bc_when_volume_then_duration_1tick) {
  auto p = new ProgramStepBleControlKey<FakeLogger, FakeHidBle>(logger, ble, "Volume+");

  assertEqual(1, p->duration());

  p->dispose();
  delete p;
}

test(given_ut_when_empty_then_duration_at_least_1ticks) {
  auto p = new ProgramStepUsbKeyboardText<FakeLogger, FakeKeyboard>(logger, usb, "");

  assertEqual(1, p->duration());

  p->dispose();
  delete p;
}

test(given_ut_when_hello_world_then_duration_strlen_11ticks) {
  auto p = new ProgramStepUsbKeyboardText<FakeLogger, FakeKeyboard>(logger, usb, "Hello World");

  assertEqual(11, p->duration());

  p->dispose();
  delete p;
}

test(given_uk_when_any_then_duration_1ticks) {
  auto p = new ProgramStepUsbKeyboardCode<FakeLogger, FakeKeyboard>(logger, usb, 0x0F);

  assertEqual(1, p->duration());

  p->dispose();
  delete p;
}

test(given_uk_repeated_when_any_4_times_then_duration_4ticks) {
  auto p = new ProgramStepUsbKeyboardCodeRepeated<FakeLogger, FakeKeyboard>(logger, usb, 0x0F, 4);

  assertEqual(4, p->duration());

  p->dispose();
  delete p;
}

test(given_uk_codes_when_any_then_duration_1ticks) {
  auto p = new ProgramStepUsbKeyboardCodes<FakeLogger, FakeKeyboard>(logger, usb, 'k', 'd');

  assertEqual(1, p->duration());

  p->dispose();
  delete p;
}

test(given_valid_three_program_steps_when_through_then_hello_is_sent_last) {
  auto p1 = new ProgramStepBleKeyboardText<FakeLogger, FakeHidBle>(logger, ble, "Hello");
  auto p2 = new ProgramStepWait<FakeLogger>(logger, 200);
  auto p3 = new ProgramStepBleKeyboardCode<FakeLogger, FakeHidBle>(logger, ble, 0x1);
  p1->appendStep(p2);
  p2->appendStep(p3);

  Program<FakeLogger> p;
  p.appendStep(p1);

  p.play(); // 0 BT Hello
  p.play(); // 1
  p.play(); // 2
  p.play(); // 3
  p.play(); // 4
  p.play(); // 5
  p.play(); // 6
  p.play(); // 7
  p.play(); // 8
  p.play(); // 9
  p.play(); // 10 => 0 W 200
  p.play(); // 1
  p.play(); // 2 => 0 BK 0x1
  p.play(); // 1
  p.play(); // 2
  p.play(); // 3
  p.play(); // 4
  p.play(); // 5 => 0 rewind is required
  p.play(); // 5 => 0 
  p.play(); // 5 => 0 
  p.rewind();
  p.play(); // 0 BT Hello

  assertEqual(0, strcmp("Hello", ble.lastPrintln));

  p.dispose();
}

void setup() {
  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  Test::run();
}
