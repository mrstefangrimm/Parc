// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parc {

  // Constants to avoids multiple usage of GPIOs.
  // No additional memory is used because of the external constants.
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

}
