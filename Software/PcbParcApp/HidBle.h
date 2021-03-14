// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitConfig.h"

namespace pcbparc { 
  
  template<typename TLOGGER>
  class HidBle {
  public:
    HidBle(TLOGGER& logger)
      : _log(logger), _ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST) {}

    void begin(bool verbose = false, bool factoryReset = false) {

      if (!_ble.begin(verbose)) {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
      }
      if (factoryReset) {
        _log.println(F("Performing a factory reset (output on Serial): "));
        if (!_ble.factoryReset()) {
          error(F("Factory reset failed!"));
        }
      }
      _ble.echo(false);

      _ble.info();

      if (!_ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION)) {
        error(F("This sketch requires a higher firmware version."));
      }

      _log.println(F("Enable HID Services (including Control Key): "));
      if (!_ble.sendCommandCheckOK(F("AT+BLEHIDEN=On"))) {
        error(F("Failed to enable HID."));
      }

      _log.println(F("Performing a SW reset (service changes require a reset): "));
      if (!_ble.reset()) {
        error(F("Couldn't reset??"));
      }
    }   

    bool sendKeyCode(KeyCode keyCode) {
      // Debug: _log.print(F("Sending keycode mem before: ")); _log.println(freeMemory());
      uint8_t modifier = keyCode.ctrl == true;
      if (keyCode.shift) { modifier |= 0x2; }
      if (keyCode.alt) { modifier |= 0x4; }
      if (keyCode.win) { modifier |= 0x8; }
      _ble.print(F("AT+BLEKEYBOARDCODE="));
      if (modifier < 0x10) { _ble.print("0"); }
      _ble.print(modifier, HEX);
      _ble.print(F("-00-"));
      if (keyCode.hexCode < 0x10) { _ble.print(F("0")); }
      _ble.print(keyCode.hexCode, HEX);
      _ble.println(F("-00-00-00-00"));

      if (_ble.sendCommandCheckOK(F("AT+BLEKEYBOARDCODE=00-00"))) {
        // Debug: _log.print(F("Sending keycode mem after: ")); _log.println(freeMemory());
        return true;
      }
      _log.println(F("Send command failed. Enable verbose mode for more information."));
      // Debug: _log.print(F("Sending keycode mem after: ")); _log.println(freeMemory());
      return false;
    }    

    void info() {
      _log.println(F("BluefruitLE info (output on Serial): "));
      _ble.info();

      _log.println(F("List of pre-defined control keys:"));
      _log.print(F(
        "- BRIGHTNESS+" "\n"
        "- BRIGHTNESS-" "\n"
        "- PLAYPAUSE" "\n"
        "- MEDIANEXT" "\n"
        "- MEDIAPREVIOUS" "\n"
        "- MEDIASTOP" "\n"
        "- VOLUME" "\n"
        "- MUTE" "\n"
        "- BASS" "\n"
        "- TREBLE" "\n"
        "- BASS_BOOST" "\n"
        "- VOLUME+" "\n"
        "- VOLUME-" "\n"
        "- BASS+" "\n"
        "- BASS-" "\n"
        "- TREBLE+" "\n"
        "- TREBLE-" "\n"
        "- EMAILREADER" "\n"
        "- CALCULATOR" "\n"
        "- FILEBROWSER" "\n"
        "- SEARCH" "\n"
        "- HOME" "\n"
        "- BACK" "\n"
        "- FORWARD" "\n"
        "- STOP" "\n"
        "- REFRESH" "\n"
        "- BOOKMARKS" "\n"
      ));
    }
    
    bool waitForOK() {
      return _ble.waitForOK();
    }

    void print(const char* const data) {
      _ble.print(data);
    }
    void print(const __FlashStringHelper* data) {
      _ble.print(data);
    }

    void println(const char* const data) {
      _ble.println(data);
    }
    void println(const __FlashStringHelper* data) {
      _ble.println(data);
    }

  private:
    void error(const __FlashStringHelper* err) {
      _log.println(err);
      while(true);
    }

    const char* MINIMUM_FIRMWARE_VERSION = "0.6.6";
    Adafruit_BluefruitLE_SPI _ble;
    TLOGGER& _log;
  };

}
