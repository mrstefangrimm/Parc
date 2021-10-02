// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using System.Threading;
using WinParc.Core;
using WinParc.View;

public class WinParcAPI {  

  Thread gui_thread;
  MainWindow mainview_ui;
  Status status;

  public void Initialize() {
    gui_thread = new Thread(() => {
      mainview_ui = new MainWindow();
      status = new Status();
      mainview_ui.DataContext = new MainWindowViewModel(status);
      mainview_ui.Show();
      System.Windows.Threading.Dispatcher.Run();
    });
    gui_thread.SetApartmentState(ApartmentState.STA); // STA Thread Initialization
    gui_thread.Start();
  }

  public bool KeypadPressed(string button) {
    return status != null && status.GetKeyPressed(button);
  }

  public bool TerminalIsAvailable() {
    return status != null && status.IsTerminalInputAvailable;
  }

  public int TerminalRead() {
    return status.ReadTerminalInput();
  }

  public void TerminalPrint(string text) {
    status.SetTerminalOutput(text);
  }

  public void TerminalPrintLn(string text) {
    status.SetTerminalOutput(text + System.Environment.NewLine);
  }

  public void DebugPrint(string text) {
    status.SetDebugOutput(text);
  }

  public void DebugPrintLn(string text) {
    status.SetDebugOutput(text + System.Environment.NewLine);
  }

  public void SetWarnLed(bool on) {
    if (status != null) {
      status.IsWarnLedOn = on;
    }
  }

}
