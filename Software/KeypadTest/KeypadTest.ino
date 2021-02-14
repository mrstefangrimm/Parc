// Written by Stefan Grimm, 2021.
// Released into the public domain.
//
#include <SoftwareSerial.h>
#include "Adafruit_MCP23008.h"

const uint8_t Btn_A = 7;
const uint8_t Btn_B = 6;
const uint8_t Btn_C = 5;
const uint8_t Btn_D = 4;
const uint8_t Btn_E = 0;
const uint8_t Btn_P0 = 1;
const uint8_t Btn_P1 = 2;

const uint8_t Code_0 = 10;
const uint8_t Code_1 = 9;
const uint8_t Code_2 = 6;
const uint8_t Code_3 = 5;

const uint8_t Usb_YEL = 11;
const uint8_t Usb_ORA = 12;

typedef SoftwareSerial Logger_t;
Logger_t logger(Usb_ORA, Usb_YEL);
Adafruit_MCP23008 mcp;
  
void setup() {
  for (int n=0; n<5 && !Serial; n++) {
    delay(100);
  }
  Serial.begin(9600);
  logger.begin(9600);
  
  mcp.begin();
  mcp.pinMode(Btn_A, INPUT); mcp.pullUp(Btn_A, HIGH);
  mcp.pinMode(Btn_B, INPUT); mcp.pullUp(Btn_B, HIGH);
  mcp.pinMode(Btn_C, INPUT); mcp.pullUp(Btn_C, HIGH);
  mcp.pinMode(Btn_D, INPUT); mcp.pullUp(Btn_D, HIGH);
  mcp.pinMode(Btn_E, INPUT); mcp.pullUp(Btn_E, HIGH);
  mcp.pinMode(Btn_P0, INPUT); mcp.pullUp(Btn_P0, HIGH);
  mcp.pinMode(Btn_P1, INPUT); mcp.pullUp(Btn_P1, HIGH);

  ::pinMode(Code_0, INPUT_PULLUP);
  ::pinMode(Code_1, INPUT_PULLUP);
  ::pinMode(Code_2, INPUT_PULLUP);
  ::pinMode(Code_3, INPUT_PULLUP);
}

void loop() {
  if (mcp.digitalRead(Btn_A) == LOW) {
    Serial.println("Btn_A");
    logger.println("Btn_A");
  }
  if (mcp.digitalRead(Btn_B) == LOW) {
    Serial.println("Btn_B");
    logger.println("Btn_B");
  }
  if (mcp.digitalRead(Btn_C) == LOW) {
    Serial.println("Btn_C");
    logger.println("Btn_C");
  }
  if (mcp.digitalRead(Btn_D) == LOW) {
    Serial.println("Btn_D");
    logger.println("Btn_D");
  }
  if (mcp.digitalRead(Btn_E) == LOW) {
    Serial.println("Btn_E");
    logger.println("Btn_E");
  }
  if (mcp.digitalRead(Btn_P0) == LOW) {
    Serial.println("Btn_P0");
    logger.println("Btn_P0");
  }
  if (mcp.digitalRead(Btn_P1) == LOW) {
    Serial.println("Btn_P1");
    logger.println("Btn_P1");
  }
  if (::digitalRead(Code_0) == HIGH) {
    Serial.println("Code_0");
    logger.println("Code_0");
  }
  if (::digitalRead(Code_1) == HIGH) {
    Serial.println("Code_1");
    logger.println("Code_1");
  }
  if (::digitalRead(Code_2) == HIGH) {
    Serial.println("Code_2");
    logger.println("Code_2");
  }
  if (::digitalRead(Code_3) == HIGH) {
    Serial.println("Code_3");
    logger.println("Code_3");
  }
  delay(500);  
}
