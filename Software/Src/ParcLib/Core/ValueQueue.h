// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

extern void assert(bool);

namespace parclib {

  // ItemAlloc, pop return false?
  template<size_t size, class ItemType = uint8_t>
  class ValueQueue {
  public:
    void push(ItemType item) {
      //static_assert(false, "Only queue size 1 and 2 is supported");
    }
    ItemType pop() {
      return 0;
    }
  };

  template<class ItemType>
  class ValueQueue<0, ItemType> {
  public:
    void push(ItemType item) {
      //static_assert(false, "Only queue size 1 and 2 is supported");
    }
    ItemType pop() {
      return 0;
    }
  private:
    ItemType _item = 0;
  };

  template<class ItemType>
  class ValueQueue<1, ItemType> {
  public:
    void push(ItemType item) {
      assert(_item == 0);
      _item = item;
    }
    ItemType pop() {
      ItemType ret = _item;
      _item = 0;
      return ret;
    }
  private:
    ItemType _item = 0;
  };

  template<class ItemType>
  class ValueQueue<2, ItemType> {
  public:
    void push(ItemType item) {
      assert(_item1 == 0 || _item2 == 0);
      if (_item1 == 0) {
        _item1 = item;
      }
      else {
        _item2 = item;
      }

    }
    ItemType pop() {
      ItemType ret = _item1;
      _item1 = _item2;
      _item2 = 0;
      return ret;
    }
  private:
    ItemType _item1 = 0;
    ItemType _item2 = 0;
  };


  template<size_t Timerbits, class Alloc = uint8_t>
  struct BitTimer {

    enum { N = Timerbits };

    BitTimer() : raw(0) {}
    explicit BitTimer(Alloc countDown) : raw((1 << N) - countDown) {}

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
  struct BitTimer<0, Alloc> {

    BitTimer() {}

    bool increment() {
      return true;
    }

    bool current() {
      return true;
    }
  };
}