// Copyright (c) 2023-2024 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using Futile.Specflow.Actions.FlaUI;

namespace WinParc.App.Specs.SimulatorApp {

  public class ParcSimulator {

    private readonly ParcSimulatorElements _elements;

    public ParcSimulator(FlaUIDriver driver) {
      _elements = new ParcSimulatorElements(driver);
    }

    public void SendCommand(string command) {
      _elements.SendText.Text = command;
      _elements.SendPressCmd.Click();
    }

    public string GetTerminalOutput() {
      return _elements.TerminalOutput.Text;
    }

    public string GetDebugOutput() {
      return _elements.DebugOutput.Text;
    }

    public void SetMode(bool m1, bool m0) {
      if (m1) _elements.CheckBoxM1.Click();
      if (m0) _elements.CheckBoxM0.Click();
    }

    public void SetCode(bool c4, bool c3, bool c2, bool c1) {
      if (c4) _elements.CheckBoxC4.Click();
      if (c3) _elements.CheckBoxC3.Click();
      if (c2) _elements.CheckBoxC2.Click();
      if (c1) _elements.CheckBoxC1.Click();
    }

    public void PressButton(string button) {
      switch (button) {
        case "A": _elements.ButtonA.Click(); break;
        case "B": _elements.ButtonB.Click(); break;
        case "C": _elements.ButtonC.Click(); break;
        case "D": _elements.ButtonD.Click(); break;
        case "E": _elements.ButtonE.Click(); break;
        default: throw new System.NotImplementedException();
      }
    }
  }
}
