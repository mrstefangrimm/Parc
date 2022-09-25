// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace pcbparc {

// Constants to avoids multiple usage of GPIOs.
// No additional memory is used because of the external constants.

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

}
