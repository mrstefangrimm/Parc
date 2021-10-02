// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using System;
using System.Collections.Generic;

namespace WinParc.Core {

  public enum Button {
    A,
    B,
    C,
    D,
    E,
    M1,
    M0,
    C4,
    C3,
    C2,
    C1
  }

  public class OutputEventArgs : EventArgs {
    public string Message { get; set; }
  }

  public class WarnStatusEventArgs : EventArgs {
    public bool Warn { get; set; }
  }

  public class Status {

    private object _lockObject = new object();
    private string _input = string.Empty;
    private bool _isWarnLedOn;
    private Dictionary<string, bool> _pressedKeys = new Dictionary<string, bool>();

    public event EventHandler<OutputEventArgs> DebugOutputChanged;
    public event EventHandler<OutputEventArgs> TerminalOutputChanged;
    public event EventHandler<WarnStatusEventArgs> WarnStatusChanged;

    public bool IsWarnLedOn {
      get { return _isWarnLedOn; }
      set {
        _isWarnLedOn = value;
        WarnStatusChanged?.Invoke(this, new WarnStatusEventArgs { Warn = value });
      }
    }

    public void SetTerminalCommand(string input) {
      _input = input;
    }

    public bool IsTerminalInputAvailable {
      get {
        return _input.Length > 0;
      }
    }

    public char ReadTerminalInput() {
      char retVal = '\0';
      if (_input.Length > 0) {
        retVal = _input[0];
        _input = _input.Remove(0, 1);
      }
      return retVal;
    }

    public void SetTerminalOutput(string output) {
      TerminalOutputChanged?.Invoke(this, new OutputEventArgs { Message = output });
    }

    public void SetDebugOutput(string output) {
      DebugOutputChanged?.Invoke(this, new OutputEventArgs { Message = output });
    }  

    public void SetKeyPressed(Button key, bool pressed) {
      lock (_lockObject) {
        _pressedKeys[key.ToString()] = pressed;
      }
    }

    public bool GetKeyPressed(string key) {
      lock (_lockObject) {
        bool isPressed = _pressedKeys.ContainsKey(key) && _pressedKeys[key];
        if (isPressed) {
          _pressedKeys[Button.A.ToString()] = false;
          _pressedKeys[Button.B.ToString()] = false;
          _pressedKeys[Button.C.ToString()] = false;
          _pressedKeys[Button.D.ToString()] = false;
          _pressedKeys[Button.E.ToString()] = false;
        }
        return isPressed;
      }
    }

  }
}
