// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>
#include "ParcLib.h"

#define BEGIN(a)
#define END
#define TEST(given, when, then) test(GIVEN_##given##_WHEN_##when##_THEN_##then)
#define EQ(a, b) assertEqual(a, b)
#define NN(a)

class FakeSerial {
public:
  void setInputBuffer(const char* terminalInput) {
    for (size_t n = 0; n < strlen(terminalInput); n++) {
      //_buf.push_back(terminalInput[n]);
    }
  }

  template<class T>
  void print(T ch) {}
  template<class T>
  void print(T ch, int mode) {}
  template<class T>
  void println(T ch) {}
  template<class T>
  void println(T ch, int mode) {}
  void println() {}

  bool available() {
    true;
  }
  int read() {

    return -1;
  }

private:
} FSerial;


#include "TerminalAoTest.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
}

void loop() {
  Test::run();
}
