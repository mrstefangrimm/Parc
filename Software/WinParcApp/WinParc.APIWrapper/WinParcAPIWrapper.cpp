// Copyright (c) 2021 Stefan Grimm. All rights reserved.
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


//double YahooAPIWrapper::GetBid(const char* symbol)
//{
//    return _private->yahooAPI->GetBid(gcnew System::String(symbol));
//}
//
//double YahooAPIWrapper::GetAsk(const char* symbol)
//{
//    return _private->yahooAPI->GetAsk(gcnew System::String(symbol));
//}
//
//const char* YahooAPIWrapper::GetCapitalization(const char* symbol)
//{
//    System::String^ managedCapi = _private->yahooAPI->GetCapitalization(gcnew System::String(symbol));
//
//    return (const char*)Marshal::StringToHGlobalAnsi(managedCapi).ToPointer();
//}
//
//const char** YahooAPIWrapper::GetValues(const char* symbol, const char* fields)
//{
//    cli::array<System::String^>^ managedValues = _private->yahooAPI->GetValues(gcnew System::String(symbol), gcnew System::String(fields));
//
//    const char** unmanagedValues = new const char*[managedValues->Length];
//
//    for (int i = 0; i < managedValues->Length; ++i)
//    {
//        unmanagedValues[i] = (const char*)Marshal::StringToHGlobalAnsi(managedValues[i]).ToPointer();
//    }
//
//    return unmanagedValues;
//}

