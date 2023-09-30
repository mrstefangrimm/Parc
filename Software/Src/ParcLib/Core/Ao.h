// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Messages.h"

namespace parclib {

template<class TDERIVED>
class Ao {
  public:
    Ao(Messages& messages)
      : _messages(messages) {}

    void load() {
      static_cast<TDERIVED*>(this)->load();
    }

    void run() {
      static_cast<TDERIVED*>(this)->run();
    }

  protected:
    Messages& _messages;
};

}
