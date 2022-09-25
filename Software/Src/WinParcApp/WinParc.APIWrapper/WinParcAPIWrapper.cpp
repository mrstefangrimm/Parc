// Copyright (c) 2021-2022 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#using "WinParcAPI.dll"

#include <msclr\auto_gcroot.h>
#include "WinParcAPIWrapper.h"

using namespace System::Runtime::InteropServices;

class WinParcAPIWrapperImpl {
public:
  WinParcAPIWrapperImpl() {
    _winParcAPI = gcnew WinParcAPI();
  }
  WinParcAPI^ Get() { return _winParcAPI.get(); }

private:
  msclr::auto_gcroot<WinParcAPI^> _winParcAPI;
};

WinParcAPIWrapper::WinParcAPIWrapper() {
  _impl = new WinParcAPIWrapperImpl();
}

WinParcAPIWrapper::~WinParcAPIWrapper() {
  delete _impl;
}

void WinParcAPIWrapper::Initialize() {
  _impl->Get()->Initialize();
}

bool WinParcAPIWrapper::KeypadPressed(const char* button) {
  return _impl->Get()->KeypadPressed(gcnew System::String(button));
}

bool WinParcAPIWrapper::TerminalIsAvailable() {
  return _impl->Get()->TerminalIsAvailable();
}

int WinParcAPIWrapper::TerminalRead() {
  return _impl->Get()->TerminalRead();
}

void WinParcAPIWrapper::TerminalPrint(const char* text) {
  _impl->Get()->TerminalPrint(gcnew System::String(text));
}

void WinParcAPIWrapper::TerminalPrintLn(const char* text) {
  _impl->Get()->TerminalPrintLn(gcnew System::String(text));
}

void WinParcAPIWrapper::DebugPrint(const char* text) {
  _impl->Get()->DebugPrint(gcnew System::String(text));
}

void WinParcAPIWrapper::DebugPrintLn(const char* text) {
  _impl->Get()->DebugPrintLn(gcnew System::String(text));
}

void WinParcAPIWrapper::SetWarnLed(bool on) {
  _impl->Get()->SetWarnLed(on);
}
