// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"

#include <iostream>
#include <list>

#define F(x) (x)

const int INPUT_PULLUP = 1;
const int OUTPUT = 2;

enum SerialFakeMode { BIN, HEX };

class FakeSerial {
public:
  void setInputBuffer(const char* terminalInput) {
    for (size_t n = 0; n < strlen(terminalInput); n++) {
      _buf.push_back(terminalInput[n]);
    }
  }

  template<class T>
  void print(T ch) { std::cout << ch; }
  template<class T>
  void print(T ch, SerialFakeMode mode) { std::cout << ch; }
  template<class T>
  void println(T ch) { std::cout << ch << std::endl; }
  template<class T>
  void println(T ch, SerialFakeMode mode) { cout << ch << endl; }
  void println() { std::cout << std::endl; }

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
  std::list<char> _buf;
} FSerial;


#include "../../ParcLib/Tests/Core/TerminalAoTest/TerminalAoTest.h"
