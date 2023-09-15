// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define BEGIN(a) TEST_CLASS(a) {
#define END };
#define INIT(name, exp) TEST_METHOD_INITIALIZE(name) exp
#define TEST(given, when, then) TEST_METHOD(GIVEN_##given##_WHEN_##when##_THEN_##then)
#define EQ(a, b) Assert::AreEqual(a, b)
#define NN(a) Assert::IsNotNull(a)

#endif //PCH_H
