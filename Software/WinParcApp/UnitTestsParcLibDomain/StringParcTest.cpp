// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#include "pch.h"
#include "CppUnitTest.h"

namespace StringParcTest {

#include "Domain/StringParc.h"

  using namespace Microsoft::VisualStudio::CppUnitTestFramework;
  using namespace parclib;

  // Test naming scheme: Given-When-Then

  TEST_CLASS(StringParcTest)
  {
  public:

    TEST_METHOD(given_text_with_leading_spaces_when_trimFront_then_spaces_are_removed)
    {
      char text[30];
      strcpy_s(text, "  Text ");
      parclib::trimFront(text, strlen(text));
      Assert::AreEqual('T', text[0]);
    }

    TEST_METHOD(given_text_with_trailing_spaces_when_trimBack_then_spaces_are_removed)
    {
      char text[30];
      strcpy_s(text, "  Text ");
      parclib::trimBack(text, strlen(text));
      Assert::AreEqual('t', text[strlen(text) - 1]);
    }

    TEST_METHOD(given_text_with_backspace_characters_when_squeeze_then_backspaces_are_removed) 
    {
      char text[30];
      strcpy_s(text, "Tee\bxt");
      parclib::squeeze(text);
      Assert::AreEqual(0, strcmp("Text", text));
    }

    TEST_METHOD(given_text_with_two_semicolums_when_count_then_two_are_found)
    {
      char text[30];
      strcpy_s(text, "Text; text; txt");
      int numSemis = parclib::count(text, strlen(text), ';');
      Assert::AreEqual(2, numSemis);
    }

    TEST_METHOD(given_text_with_four_spaces_one_in_textstring_when_count_then_two_is_found) 
    {
      char text[30];
      strcpy_s(text, "Text \"text text\" txt");
      int numSemis = parclib::count(text, strlen(text), ' ');
      Assert::AreEqual(2, numSemis);
    }

    TEST_METHOD(given_text_with_three_spaces_when_split_then_four_substrings_are_found) 
    {
      char text[30];
      char* subStr[4];
      uint8_t numSubStr;
      strcpy_s(text, "Text text text txt");
      parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

      Assert::AreEqual<uint8_t>(4, numSubStr);
      Assert::AreEqual(0, strcmp("Text", subStr[0]));
      Assert::AreEqual(0, strcmp("text", subStr[1]));
      Assert::AreEqual(0, strcmp("text", subStr[2]));
      Assert::AreEqual(0, strcmp("txt", subStr[3]));
    }

    TEST_METHOD(given_text_with_more_spaces_in_between_when_split_then_four_substrings_do_not_have_leading_or_trailing_spaces) 
    {
      char text[30];
      char* subStr[4];
      uint8_t numSubStr;
      strcpy_s(text, "Text  text    text      txt");
      parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

      Assert::AreEqual<uint8_t>(4, numSubStr);
      Assert::AreEqual(0, strcmp("Text", subStr[0]));
      Assert::AreEqual(0, strcmp("text", subStr[1]));
      Assert::AreEqual(0, strcmp("text", subStr[2]));
      Assert::AreEqual(0, strcmp("txt", subStr[3]));
    }

    TEST_METHOD(given_text_with_three_spaces_one_in_textstring_when_split_then_three_substrings_are_found) 
    {
      char text[30];
      char* subStr[3];
      uint8_t numSubStr;
      strcpy_s(text, "Text \"text text\" txt");
      parclib::split(text, strlen(text), ' ', subStr, &numSubStr);

      Assert::AreEqual<uint8_t>(3, numSubStr);
      Assert::AreEqual(0, strcmp("Text", subStr[0]));
      Assert::AreEqual(0, strcmp("text text", subStr[1]));
      Assert::AreEqual(0, strcmp("txt", subStr[2]));
    }

  };

}
