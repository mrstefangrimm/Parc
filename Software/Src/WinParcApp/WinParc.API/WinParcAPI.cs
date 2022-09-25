// Copyright (c) 2021-2022 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using System.Threading;
using WinParc.Core;
using WinParc.View;

public class WinParcAPI {
  private Thread _guiThread;
  private MainWindow _mainWindow;
  private Status _status;

  public void Initialize() {
    _guiThread = new Thread(() => {
      _mainWindow = new MainWindow();
      _status = new Status();
      _mainWindow.DataContext = new MainWindowViewModel(_status);
      _mainWindow.Show();
      System.Windows.Threading.Dispatcher.Run();
    });
    _guiThread.SetApartmentState(ApartmentState.STA);
    _guiThread.Start();
  }

  public bool KeypadPressed(string button) {
    return _status != null && _status.GetKeyPressed(button);
  }

  public bool TerminalIsAvailable() {
    return _status != null && _status.IsTerminalInputAvailable;
  }

  public int TerminalRead() {
    return _status.ReadTerminalInput();
  }

  public void TerminalPrint(string text) {
    _status.SetTerminalOutput(text);
  }

  public void TerminalPrintLn(string text) {
    _status.SetTerminalOutput(text + System.Environment.NewLine);
  }

  public void DebugPrint(string text) {
    _status.SetDebugOutput(text);
  }

  public void DebugPrintLn(string text) {
    _status.SetDebugOutput(text + System.Environment.NewLine);
  }

  public void SetWarnLed(bool on) {
    if (_status != null) {
      _status.IsWarnLedOn = on;
    }
  }

}
