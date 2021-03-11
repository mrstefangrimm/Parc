// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//

#line 6 "basic.ino"
#include <ArduinoUnit.h>

#include "Src/StringParc.h"

// Test naming scheme: Given-When-Then

test(given_text_with_leading_spaces_when_trimFront_then_spaces_are_removed) {
  char text[30];
  strcpy(text, "  Text ");
  parclib::trimFront(text, strlen(text));
  assertEqual('T', text[0]);
}

test(given_text_with_trailing_spaces_when_trimBack_then_spaces_are_removed) {
  char text[30];
  strcpy(text, "  Text ");
  parclib::trimBack(text, strlen(text));
  assertEqual('t', text[strlen(text) - 1]);
}

test(given_text_with_backspace_characters_when_squeeze_then_backspaces_are_removed) {
  char text[30];
  strcpy(text, "Tee\bxt");
  parclib::squeeze(text);
  assertEqual(0, strcmp("Text", text));
}

test(given_text_with_two_semicolums_when_count_then_two_are_found) {
  char text[30];
  strcpy(text, "Text; text; txt");
  int numSemis = parclib::count(text, strlen(text), ';');
  assertEqual(2, numSemis);
}

test(given_text_with_four_spaces_one_in_textstring_when_count_then_two_is_found) {
  char text[30];
  strcpy(text, "Text \"text text\" txt");
  int numSemis = parclib::count(text, strlen(text), ' ');
  assertEqual(2, numSemis);
}

test(given_text_with_three_spaces_when_split_then_four_substrings_are_found) {
  char text[30];
  char* subStr[4];
  uint8_t numSubStr;
  strcpy(text, "Text text text txt");
  parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

  assertEqual(4, numSubStr);
  assertEqual(0, strcmp("Text", subStr[0]));
  assertEqual(0, strcmp("text", subStr[1]));
  assertEqual(0, strcmp("text", subStr[2]));
  assertEqual(0, strcmp("txt", subStr[3]));
}

test(given_text_with_three_spaces_one_in_textstring_when_split_then_three_substrings_are_found) {
  char text[30];
  char* subStr[3];
  uint8_t numSubStr;
  strcpy(text, "Text \"text text\" txt");
  parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

  assertEqual(3, numSubStr);
  assertEqual(0, strcmp("Text", subStr[0]));
  assertEqual(0, strcmp("text text", subStr[1]));
  assertEqual(0, strcmp("txt", subStr[2]));
}

void setup() {
  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  Test::run();
}
