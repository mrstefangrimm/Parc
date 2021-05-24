// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

class WinParcAPIWrapperPrivate;

class __declspec(dllexport) WinParcAPIWrapper
{
private:
  WinParcAPIWrapperPrivate* _private;

public:
  WinParcAPIWrapper();
  ~WinParcAPIWrapper();

  void Initialize();

  bool KeypadPressed(const char* button);

  bool TerminalIsAvailable();

  int TerminalRead();

  void TerminalPrint(const char* text);

  void TerminalPrintLn(const char* text);

  void DebugPrint(const char* text);

  void DebugPrintLn(const char* text);

  void SetWarnLed(bool on);
  
};
