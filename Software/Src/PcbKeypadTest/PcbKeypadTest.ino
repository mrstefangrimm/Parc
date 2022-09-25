// Written by Stefan Grimm, 2021-2022.
// Released into the public domain.
//
#include <SoftwareSerial.h>
#include "Adafruit_MCP23X08.h"

// MCP expander pins
const uint8_t Pin_E = 0;
const uint8_t Pin_C1 = 1;
const uint8_t Pin_C2 = 2;
const uint8_t Pin_C3 = 3;
const uint8_t Pin_C4 = 4;
const uint8_t Pin_M0 = 6;
const uint8_t Pin_M1 = 7;

// Arduino pins
const uint8_t Pin_A = 6;
const uint8_t Pin_B = 9;
const uint8_t Pin_C = 5;
const uint8_t Pin_D = 10;

const uint8_t Usb_YEL = 11;
const uint8_t Usb_ORA = 12;

typedef SoftwareSerial Logger_t;
Logger_t logger(Usb_ORA, Usb_YEL);
Adafruit_MCP23X08 mcp;

void setup() {
  for (int n = 0; n < 50 && !Serial; n++) {
    delay(100);
  }
  Serial.begin(9600);
  logger.begin(9600);

  mcp.begin_I2C();
  mcp.pinMode(Pin_E, INPUT_PULLUP);
  mcp.pinMode(Pin_C1, INPUT_PULLUP);
  mcp.pinMode(Pin_C2, INPUT_PULLUP);
  mcp.pinMode(Pin_C3, INPUT_PULLUP);
  mcp.pinMode(Pin_C4, INPUT_PULLUP);
  mcp.pinMode(Pin_M0, INPUT_PULLUP);
  mcp.pinMode(Pin_M1, INPUT_PULLUP);

  ::pinMode(Pin_A, INPUT_PULLUP);
  ::pinMode(Pin_B, INPUT_PULLUP);
  ::pinMode(Pin_C, INPUT_PULLUP);
  ::pinMode(Pin_D, INPUT_PULLUP);
  ::pinMode(Pin_M0, INPUT_PULLUP);
  ::pinMode(Pin_M1, INPUT_PULLUP);
}

void loop() {
  if (mcp.digitalRead(Pin_E) == LOW) {
    Serial.println("Btn_E");
    logger.println("Btn_E");
  }
  if (mcp.digitalRead(Pin_C1) == LOW) {
    Serial.println("Code_1");
    logger.println("Code_1");
  }
  if (mcp.digitalRead(Pin_C2) == LOW) {
    Serial.println("Code_2");
    logger.println("Code_2");
  }
  if (mcp.digitalRead(Pin_C3) == LOW) {
    Serial.println("Code_3");
    logger.println("Code_3");
  }
  if (mcp.digitalRead(Pin_C4) == LOW) {
    Serial.println("Code_4");
    logger.println("Code_4");
  }
  if (mcp.digitalRead(Pin_M0) == LOW) {
    Serial.println("Btn_M0");
    logger.println("Btn_M0");
  }
  if (mcp.digitalRead(Pin_M1) == LOW) {
    Serial.println("Btn_M1");
    logger.println("Btn_M1");
  }
  if (::digitalRead(Pin_A) == LOW) {
    Serial.println("Btn_A");
    logger.println("Btn_A");
  }
  if (::digitalRead(Pin_B) == LOW) {
    Serial.println("Btn_B");
    logger.println("Btn_B");
  }
  if (::digitalRead(Pin_C) == LOW) {
    Serial.println("Btn_C");
    logger.println("Btn_C");
  }
  if (::digitalRead(Pin_D) == LOW) {
    Serial.println("Btn_D");
    logger.println("Btn_D");
  }

  delay(500);
}
