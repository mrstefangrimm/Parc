// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

/*Begin test scenario.*/
#define BEGIN(a)

/*End test scenario.*/
#define END

/*Initialize test with the definition.*/
#define INIT(name, definition)

/*Test name followed by the definition.*/
#define TEST(given, when, then) test(GIVEN_##given##_WHEN_##when##_THEN_##then)

/*Assert are equal.*/
#define EQ(a, b) assertEqual(a, b)

/*Assert are equal.*/
#define NN(a) assertEqual(true, (a != nullptr))
