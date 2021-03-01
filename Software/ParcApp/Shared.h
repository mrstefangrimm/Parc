// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

namespace parc {

  // Intervals per ActiveObject (BSP_Tick)
  const uint8_t TimerPeriod = 100;

  // Given by 5 buttons (A-E) and 4 profiles (0-3)
  const uint8_t NumberOfPrograms = 20;
  
  // Should it be in this file?
  // 20210209 - having hexCode in a separate argument uses the same amount of memory.
  struct KeyCode {
    KeyCode() : raw(0) {}
    KeyCode(const KeyCode& t) : raw(t.raw) {}
    KeyCode(uint16_t rawValue) : raw(rawValue) {}

    union {
      struct {
        uint16_t hexCode : 8;
        uint16_t ctrl    : 1;
        uint16_t shift   : 1;
        uint16_t alt     : 1;
        uint16_t win     : 1;
      };
      uint16_t raw;
    };
  };

  // From Modern C++ 
  template<uint8_t V>
  struct Int2Type {
    enum { value = V };
  };

  // From Modern C++ 
  template<bool> struct CTAssert;
  template<> struct CTAssert<true> {};
  
  // From Modern C++ 
  class NullType {};

  // From Modern C++ 
  template<class T, typename U>
  struct Typelist {
    typedef T Head;
    typedef U Tail;
  };

  // From Modern C++ 
  template<class TL, uint8_t INDEX> struct TypeAt;
  template<class HEAD, class TAIL>
  struct TypeAt<Typelist<HEAD, TAIL>, 0>
  {
    typedef HEAD Result;
  };
  template<class HEAD, class TAIL, uint8_t INDEX>
  struct TypeAt<Typelist<HEAD, TAIL>, INDEX>
  {
    typedef typename TypeAt<TAIL, INDEX - 1>::Result Result;
  };

  // From Modern C++ 
  template<class TLIST, class T> struct IndexOf;
  template<class T>
  struct IndexOf<NullType, T> {
    enum { Result = -1 };
  };
  template<class T, class TAIL>
  struct IndexOf<Typelist<T, TAIL>, T> {
    enum { Result = 0 };
  };
  template<class HEAD, class TAIL, class T>
  struct IndexOf<Typelist<HEAD, TAIL>, T> {
  private:
    enum { Temp = IndexOf<TAIL, T>::Result };
  public:
    enum { Result = Temp == -1 ? -1 : 1 + Temp };
  };
  
}
