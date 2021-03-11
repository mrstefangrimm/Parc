// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parclib {

  void trimFront(char* buf, uint8_t bufLen) {
    uint8_t pos = 0;
    while (buf[pos] == ' ' && buf[pos] != '\0') pos++;
    if (pos > 0) {
      // Depends on the implemation of memcpy.
      memcpy(buf, &buf[pos], bufLen - pos);
    }
  }

  void trimBack(char* buf, uint8_t bufLen) {
    for (uint8_t n = bufLen - 1; n > 0; n--) {
      if (buf[n] == ' ') { buf[n] = 0; }
      if (buf[n] != 0) { break; }
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
      if (buf[n] == 0) { return cnt; }
      else if (buf[n] == '"') { enclosedText = !enclosedText; }
      else if (!enclosedText && buf[n] == seperator) { cnt++; }
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
      if (buf[n] == '"') { enclosedText = !enclosedText; }
      else if (!enclosedText && buf[n] == seperator) {
        buf[n] = 0;
        n++;
        if (buf[n] != '"') {
          subStr[subStrIdx] = &buf[n];
        }
        else {
          // Remove " at the front and at tbe back.
          buf[n] = 0;
          n++;
          subStr[subStrIdx] = &buf[n];
          do { n++; } while (buf[n] != '"');
          buf[n] = 0;
        }
        subStrIdx++;
      }
    }
  }

}
