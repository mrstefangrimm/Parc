// Copyright (c) 2021-2022 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#using "WinParcAPI.dll"

#include <msclr\auto_gcroot.h>
#include "WinParcAPIWrapper.h"

using namespace System::Runtime::InteropServices;

class WinParcAPIWrapperPrivate
{
    public: msclr::auto_gcroot<WinParcAPI^> winParcAPI;
};

WinParcAPIWrapper::WinParcAPIWrapper()
{
    _private = new WinParcAPIWrapperPrivate();
    _private->winParcAPI = gcnew WinParcAPI();
}

WinParcAPIWrapper::~WinParcAPIWrapper()
{
  delete _private;
}

void WinParcAPIWrapper::Initialize()
{
  _private->winParcAPI->Initialize();
}

bool WinParcAPIWrapper::KeypadPressed(const char* button)
{
  return _private->winParcAPI->KeypadPressed(gcnew System::String(button));
}

bool WinParcAPIWrapper::TerminalIsAvailable()
{
  return _private->winParcAPI->TerminalIsAvailable();
}

int WinParcAPIWrapper::TerminalRead()
{
  return _private->winParcAPI->TerminalRead();
}

void WinParcAPIWrapper::TerminalPrint(const char* text)
{
   _private->winParcAPI->TerminalPrint(gcnew System::String(text));
}

void WinParcAPIWrapper::TerminalPrintLn(const char* text)
{
  _private->winParcAPI->TerminalPrintLn(gcnew System::String(text));
}

void WinParcAPIWrapper::DebugPrint(const char* text)
{
  _private->winParcAPI->DebugPrint(gcnew System::String(text));
}

void WinParcAPIWrapper::DebugPrintLn(const char* text)
{
  _private->winParcAPI->DebugPrintLn(gcnew System::String(text));
}

void WinParcAPIWrapper::SetWarnLed(bool on)
{
  _private->winParcAPI->SetWarnLed(on);
}
