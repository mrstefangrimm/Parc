// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parclib {

  template<size_t Timerbits, class Alloc = uint8_t>
  struct BitTimer {

    enum { N = Timerbits };

    BitTimer() : raw(0) {}
    explicit BitTimer(Alloc rawValue) : raw(rawValue) {}

    bool increment() {
      return ++value == 0;
    }

    union {
      struct {
        Alloc value : N;
      };
      Alloc raw;
    };
  };

  template<class Alloc>
  struct BitTimer<0, Alloc> {

    BitTimer() {}

    bool increment() {
      return true;
    }
  };
}