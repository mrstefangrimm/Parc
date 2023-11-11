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

namespace TerminalAoVariousTest {

using namespace parclib;
using namespace TerminalAoTestShared;

BEGIN(TerminalAoVariousTest)

void reset() {
  messages.fromKeypadToTerminalQueue.pop();
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
  EQ((uint8_t)PsType::Wait, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  valid_help_and_w,
  read,
  state_is_idle) {

  reset();

  serial.setInputBuffer("? { 0 A: W 1000; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::Wait, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

TEST(
  invalid_command_no_semicolon,
  read,
  state_is_idle_and_no_added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: W 1000 }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  FALSE(programs[0].hasSteps());
}

TEST(
  two_steps,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 A: W 1000; W 1000; }");

  for (int n = 0; n < 100; n++) {
    terminal.load();
    terminal.run();

    messages.fromTerminalToServiceMonitorQueue.pop();
  }

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::Wait, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
  EQ((uint8_t)PsType::Wait, (uint8_t) static_cast<FakeProgramStep*>(static_cast<FakeProgramStep*>(programs[0].root)->next())->type);
}

TEST(
  manual_input,
  read,
  added_step) {

  reset();

  serial.setInputBuffer("{ 0 ");
  serialInput("A: W");
  serialInput(" 1000;");
  serialInput(" }");

  EQ((uint8_t)TestTerminalAo::State::Idle, (uint8_t)terminal.getState());
  TRUE(programs[0].hasSteps());
  EQ((uint8_t)PsType::Wait, (uint8_t) static_cast<FakeProgramStep*>(programs[0].root)->type);
}

void serialInput(const char* input) {

  serial.sendInputBuffer(input);

  // process
  for (int n = 0; n < 5; n++) {
    terminal.load();
    terminal.run();
  }

}

END
}