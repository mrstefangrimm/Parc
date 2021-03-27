// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

struct FakeLogger {
  template<class T>
  void print(T ch) { }
  template<class T>
  void print(T ch, uint8_t mode) {}
  template<class T>
  void println(T ch) {}
};
