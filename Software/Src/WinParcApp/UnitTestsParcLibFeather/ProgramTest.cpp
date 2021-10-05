// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"
#include "CppUnitTest.h"

#define F(x) (x)
#define max(a,b) ((a)>(b)?(a):(b))
enum SerialFakeMode { BIN, HEX };
const int INPUT_PULLUP = 1;
const int OUTPUT = 2;

#include "Core/Shared.h"
#include "Core/Program.h"
#include "Feather/ProgramSteps.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace parclib;
using namespace std;

namespace ProgramTest {

  struct FakeLogger {
    template<class T>
    void print(T ch) { }
    template<class T>
    void print(T ch, uint8_t mode) { }
    template<class T>
    void println(T ch) { }
  };
  
  struct FakeUsbKeyboard {
    FakeUsbKeyboard() : printInvokeCounter(0) {}
   
    int printInvokeCounter;
    void print(const char* ch) { printInvokeCounter++; }
    void press(uint8_t val) { }
    void releaseAll() { }

  private:
    void println(const char* ch) { }
  };

  struct FakeHidBle {
    FakeHidBle() : printlnInvokeCounter(0) {}
    bool sendKeyCode(KeyCode keyCode) {
      return false;
    }
    bool waitForOK() {
      return true;
    }

    void print(const char*) { }
  
    int printlnInvokeCounter;
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

  TEST_CLASS(ProgramTest)
  {
  public:

    TEST_METHOD(given_w_when_5000ms_then_50ticks) {
      auto p = new ProgramStepWait<LoggerFac_t>(5000);

      Assert::AreEqual<uint8_t>(50, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_bt_when_hello_then_duration_at_least_10ticks) {
      auto p = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello");
      
      Assert::AreEqual<uint8_t>(10, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_bt_when_hello_world_then_duration_strlen_11ticks) {
      auto p = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello World");

      Assert::AreEqual<uint8_t>(11, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_bt_when_hello_world_played_then_print_is_invoked) {
      auto p = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello World");

      uint8_t tick = 0;
      p->play(tick);
      Assert::AreEqual<int>(1, ble.printlnInvokeCounter);

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_bk_when_any_then_duration_5ticks) {
      auto p = new ProgramStepBleKeyboardCode<LoggerFac_t, HidBleFac_t>(0x0F);

      Assert::AreEqual<uint8_t>(5, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_bk_repeated_when_any_4_times_then_duration_20ticks) {
      auto p = new ProgramStepBleKeyboardCodeRepeated<LoggerFac_t, HidBleFac_t>(0x0F, 4);

      Assert::AreEqual<uint8_t>(20, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_bc_when_mute_then_duration_1tick) {
      auto p = new ProgramStepBleControlKey<LoggerFac_t, HidBleFac_t>("Mute");

      Assert::AreEqual<uint8_t>(1, p->duration());

      p->dispose();
      delete p;
    }   

    TEST_METHOD(given_bc_when_volume_then_duration_1tick) {
      auto p = new ProgramStepBleControlKey<LoggerFac_t, HidBleFac_t>("Volume+");

      Assert::AreEqual<uint8_t>(1, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_ut_when_empty_then_duration_at_least_1ticks) {
      auto p = new ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>("");

      Assert::AreEqual<uint8_t>(1, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_ut_when_hello_world_then_duration_strlen_11ticks) {
      auto p = new ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>("Hello World");

      Assert::AreEqual<uint8_t>(11, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_ut_when_hello_world_played_then_print_is_invoked) {
      auto p = new ProgramStepUsbKeyboardText<LoggerFac_t, HidUsbFac_t>("Hello World");

      uint8_t tick = 0;
      p->play(tick);
      Assert::AreEqual<int>(1, usb.printInvokeCounter);

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_uk_when_any_then_duration_1ticks) {
      auto p = new ProgramStepUsbKeyboardCode<LoggerFac_t, HidUsbFac_t>(0x0F);

      Assert::AreEqual<uint8_t>(1, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_uk_repeated_when_any_4_times_then_duration_4ticks) {
      auto p = new ProgramStepUsbKeyboardCodeRepeated<LoggerFac_t, HidUsbFac_t>(0x0F, 4);

      Assert::AreEqual<uint8_t>(4, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_uk_codes_when_any_then_duration_1ticks) {
      auto p = new ProgramStepUsbKeyboardCodes<LoggerFac_t, HidUsbFac_t>('k', 'd');

      Assert::AreEqual<uint8_t>(1, p->duration());

      p->dispose();
      delete p;
    }

    TEST_METHOD(given_valid_three_program_steps_when_through_then_hello_is_sent_last) {
      auto p1 = new ProgramStepBleKeyboardText<LoggerFac_t, HidBleFac_t>("Hello");
      auto p2 = new ProgramStepWait<LoggerFac_t>(200);
      auto p3 = new ProgramStepBleKeyboardCode<LoggerFac_t, HidBleFac_t>(0x1);
      p1->appendStep(p2);
      p2->appendStep(p3);

      Program<LoggerFac_t> p;
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

      Assert::AreEqual(0, strcmp("Hello", ble.lastPrintln));

      p.dispose();
    }
  };
}
