// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parclib {

void trimFront(char* buf, uint8_t bufLen) {
  uint8_t pos = 0;
  while (buf[pos] == ' ' && buf[pos] != '\0') pos++;
  if (pos > 0) {
    int n = 0;
    while (buf[n + pos] != '\0') {
      buf[n] = buf[n + pos];
      n++;
    }
    buf[n] = '\0';
  }
}

void trimBack(char* buf, uint8_t bufLen) {
  for (uint8_t n = bufLen - 1; n > 0; n--) {
    if (buf[n] == ' ') {
      buf[n] = 0;
    }
    if (buf[n] != 0) {
      break;
    }
  }
}

void squeeze(char* buf) {
  const size_t bufLen = strlen(buf);
  uint8_t lenNew = 0;
  for (uint8_t n = 0; n < bufLen; n++) {
    if (buf[n] == 0x7F || buf[n] == '\b') {
      // backspace in putty is 0x7F; \b is just for testing
      // Debug: Serial.println(buf[n], HEX);
      lenNew--;
    }
    else {
      buf[lenNew] = buf[n];
      lenNew++;
    }
  }
  for (uint8_t n = lenNew; n < bufLen; n++) {
    buf[n] = 0;
  }
}

uint8_t count(char* buf, uint8_t bufLen, char seperator) {
  uint8_t cnt = 0;
  bool enclosedText = false;
  for (uint8_t n = 0; n < bufLen; n++) {
    if (buf[n] == 0) {
      return cnt;
    }
    else if (buf[n] == '"') {
      enclosedText = !enclosedText;
    }
    else if (!enclosedText && buf[n] == seperator) {
      cnt++;
      while (buf[n + 1] == seperator) {
        n++;
      }
    }
  }
  return cnt;
}

void split(char* buf, uint8_t bufLen, char seperator, char** subStr, uint8_t* numSubStr) {
  (*numSubStr) = parclib::count(buf, bufLen, seperator) + 1;

  // first substring points always to the begin of the buffer
  subStr[0] = buf;
  uint8_t subStrIdx = 1;
  bool enclosedText = false;
  for (uint8_t n = 1; n < bufLen && subStrIdx < (*numSubStr); n++) {
    if (buf[n] == '"' || buf[n] == '[' || buf[n] == ']') {
      enclosedText = !enclosedText;
    }
    else if (!enclosedText && buf[n] == seperator) {
      while (buf[n] == seperator) {
        buf[n] = 0;
        n++;
      }
      if (buf[n] != '"' && buf[n] != '[') {
        subStr[subStrIdx] = &buf[n];
      }
      else {
        // Remove " at the front and at tbe back.
        buf[n] = 0;
        n++;
        subStr[subStrIdx] = &buf[n];
        do {
          n++;
        } while (buf[n] != '"' && buf[n] != ']');
        buf[n] = 0;
      }
      subStrIdx++;
    }
  }
}

//strcmp2 saves program and dyn. memory, strcmp5,6,7 consume more prog. mem. than strcmp
bool strcmp2(const char* a, char b0, char b1) {
  return a[0] == b0 && a[1] == b1;
};
//inline bool strcmp5(const char* a, char b0, char b1, char b2, char b3, char b4) { return a[0] == b0 && a[1] == b1 && a[1] == b1 && a[2] == b2 && a[3] == b3 && a[4] == b4; };
//inline bool strcmp6(const char* a, char b0, char b1, char b2, char b3, char b4, char b5) { return a[0] == b0 && a[1] == b1 && a[1] == b1 && a[2] == b2 && a[3] == b3 && a[4] == b4 && a[5] == b5; };
//inline bool strcmp7(const char* a, char b0, char b1, char b2, char b3, char b4, char b5, char b6) { return a[0] == b0 && a[1] == b1 && a[1] == b1 && a[2] == b2 && a[3] == b3 && a[4] == b4 && a[5] == b5 && a[6] == b6; };

//bool contains(char* buf, uint8_t bufLen, char* cmp) {
//  uint8_t cmpLen = strlen(cmp);
//  for (uint8_t n = 0; n < bufLen - cmpLen; n++) {
//    if (strncmp(&buf[n], cmp, cmpLen) == 0) { return true; }
//    return false;
//  }
//}

}
