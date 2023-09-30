// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace StringParcTest {

#include "Core/StringParc.h"

BEGIN(StringParcTest)

TEST(
  text_with_leading_spaces,
  trimFront,
  spaces_are_removed) {
  char text[30];
  strcpy_s(text, "  Text ");
  parclib::trimFront(text);
  EQ("Text ", text);
}

TEST(
  text_with_leading_spaces_and_full_buffer,
  trimFront,
  spaces_are_removed) {
  char text[11];
  strcpy_s(text, "  Text Txt");
  parclib::trimFront(text);
  EQ("Text Txt", text);
}

TEST(
  text_with_trailing_spaces,
  trimBack,
  spaces_are_removed) {
  char text[30];
  strcpy_s(text, "  Text ");
  parclib::trimBack(text, strlen(text));
  EQ("  Text", text);
}

TEST(
  text_with_backspace_characters,
  squeeze,
  backspaces_are_removed) {
  char text[30];
  strcpy_s(text, "Tee\bxt");
  parclib::squeeze(text);
  EQ(0, strcmp("Text", text));
}

TEST(
  text_with_two_semicolums,
  count,
  two_are_found) {
  char text[30];
  strcpy_s(text, "Text; text; txt");
  int numSemis = parclib::count(text, strlen(text), ';');
  EQ(2, numSemis);
}

TEST(
  text_with_four_spaces_one_in_textstring,
  count,
  two_is_found) {
  char text[30];
  strcpy_s(text, "Text \"text text\" txt");
  int numSemis = parclib::count(text, strlen(text), ' ');
  EQ(2, numSemis);
}

TEST(
  text_with_three_spaces,
  split,
  four_substrings_are_found) {
  char text[30];
  char* subStr[4]{};
  uint8_t numSubStr;
  strcpy_s(text, "Text text text txt");
  parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

  EQ((uint8_t)4, numSubStr);
  EQ(0, strcmp("Text", subStr[0]));
  EQ(0, strcmp("text", subStr[1]));
  EQ(0, strcmp("text", subStr[2]));
  EQ(0, strcmp("txt", subStr[3]));
}

TEST(
  text_with_more_spaces_in_between,
  split,
  four_substrings_do_not_have_leading_or_trailing_spaces) {
  char text[30];
  char* subStr[4]{};
  uint8_t numSubStr;
  strcpy_s(text, "Text  text    text      txt");
  parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

  EQ((uint8_t)4, numSubStr);
  EQ(0, strcmp("Text", subStr[0]));
  EQ(0, strcmp("text", subStr[1]));
  EQ(0, strcmp("text", subStr[2]));
  EQ(0, strcmp("txt", subStr[3]));
}

TEST(
  text_with_three_spaces_one_in_textstring,
  split,
  three_substrings_are_found) {
  char text[30];
  char* subStr[3]{};
  uint8_t numSubStr;
  strcpy_s(text, "Text \"text text\" txt");
  parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

  EQ((uint8_t)3, numSubStr);
  EQ(0, strcmp("Text", subStr[0]));
  EQ(0, strcmp("text text", subStr[1]));
  EQ(0, strcmp("txt", subStr[2]));
}

END

}
