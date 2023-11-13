// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include <queue>
#include <iostream>
#include "Core/HidAo.h"
#include "Core/Shared.h"
#include "Core/Program.h"
#include "Core/KeypadAo.h"
#include "Core/TerminalAo.h"

namespace TerminalAoCommandsTest {

using namespace parclib;
using namespace TerminalAoTestShared;


BEGIN(TerminalAoCommandsTest)

void reset() {
  messages.toTerminalQueue.pop();
  messages.fromTerminalToKeypadQueue.pop();
  messages.fromKeypadToHidQueue.pop();
  messages.fromKeypadToServiceMonitorQueue.pop();
  messages.fromTerminalToServiceMonitorQueue.pop();

  // Only slot 0 is used by the tests.
  programs[0].dispose();
}

TEST(
  valid_help,
  read,
  state_is_idle) {

  reset();

  serial.setInputBuffer("?");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  FALSE(programs[0].hasSteps());
  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
}

TEST(
  valid_w,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: W 1000; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::Wait, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_uk_ctrl_k_d,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK <Ctrl> \"k d\"; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::UsbKeycodes, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_uk_l,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK 'l'; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::UsbKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_uk_win_l,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK <Win> 'l'; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::UsbKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_uk_ctrl_alt_del,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK <Ctrl> <Alt> <Del>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::UsbKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::UsbKeyCodeDel, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_uk_enter,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK <Enter>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::UsbKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::UsbKeyCodeEnter, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_uk_win_space,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK <Win> <Space>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::UsbKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::UsbKeyCodeSpace, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_uk_tab,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK <Tab>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::UsbKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::UsbKeyCodeTab, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_uk_0xFF,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK 0xFF; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::UsbKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)0xFF, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_uk_r4_10,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UK -r4 10; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::UsbKeycodeRepeated, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)16, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_bk_l,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK 'l'; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_bk_win_l,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK <Win> 'l'; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_bk_ctrl_alt_del,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK <Ctrl> <Alt> <Del>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::BleKeyCodeDel, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_bk_enter,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK <Enter>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::BleKeyCodeEnter, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_bk_win_space,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK <Win> <Space>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::BleKeyCodeSpace, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_bk_tab,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK <Tab>; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::BleKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)KnownKeycodes::BleKeyCodeTab, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_bk_0xFF,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK 0xFF; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleKeycode, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)0xFF, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_bk_r4_10,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BK -r4 10; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleKeycodeRepeated, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)16, static_cast<FakeProgramStep*>(programs[0].root)->hexCode);
}

TEST(
  valid_ut_one_word,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UT hello; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::UsbText, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_ut_two_words,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: UT \"hello parc\"; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::UsbText, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_bt_one_word,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BT hello; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleText, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_bt_two_words,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BT \"hello parc\"; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleText, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_bc_mute,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: BC Mute; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)parclib::PsType::BleControlkey, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_pin,
  read,
  set) {

  reset();

  serial.setInputBuffer("{ P N: 1 0 1 1 3; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  parclib::PinRegData regData(messages.fromTerminalToKeypadQueue.pop());
  EQ(regData.code3, (uint8_t)1);
  EQ(regData.code2, (uint8_t)0);
  EQ(regData.code1, (uint8_t)1);
  EQ(regData.code0, (uint8_t)1);
  EQ(regData.retries, (uint8_t)3);
}

END
}