// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#ifndef PARCLIB_BITCOUNTER_H
#define PARCLIB_BITCOUNTER_H

namespace parclib {

  template<size_t Timerbits, class Alloc = uint8_t>
  struct BitCounter {

    enum { N = Timerbits };

    BitCounter() : raw(0) {}
    explicit BitCounter(Alloc countDown) : raw((1 << N) - countDown) {}

    bool increment() {
      return ++value == 0;
    }

    bool current() {
      return value == 0;
    }

    union {
      struct {
        Alloc value : N;
      };
      Alloc raw;
    };
  };

  template<class Alloc>
  struct BitCounter<0, Alloc> {

    BitCounter() {}

    bool increment() {
      return true;
    }

    bool current() {
      return true;
    }
  };
}

#endif
