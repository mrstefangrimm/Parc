// Copyright (c) 2023-2024 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using FlaUI.Core.AutomationElements;
using Futile.Specflow.Actions.FlaUI;

namespace WinParc.App.Specs.SimulatorApp {

  internal class ParcSimulatorElements {

    protected readonly FlaUIDriver _driver;

    public ParcSimulatorElements(FlaUIDriver driver) {
      _driver = driver;
    }

    public TextBox SendText => _driver.Current.FindFirstDescendant("textbox_SendText").AsTextBox();

    public Button SendPressCmd => _driver.Current.FindFirstDescendant("button_SendPressCmd").AsButton();

    public TextBox TerminalOutput => _driver.Current.FindFirstDescendant("textbox_TerminalOutput").AsTextBox();

    public TextBox DebugOutput => _driver.Current.FindFirstDescendant("textbox_DebugOutput").AsTextBox();

    public Button ButtonA => _driver.Current.FindFirstDescendant("button_A").AsButton();
    public Button ButtonB => _driver.Current.FindFirstDescendant("button_B").AsButton();
    public Button ButtonC => _driver.Current.FindFirstDescendant("button_C").AsButton();
    public Button ButtonD => _driver.Current.FindFirstDescendant("button_D").AsButton();
    public Button ButtonE => _driver.Current.FindFirstDescendant("button_E").AsButton();

    public CheckBox CheckBoxM0 => _driver.Current.FindFirstDescendant("checkbox_M0").AsCheckBox();
    public CheckBox CheckBoxM1 => _driver.Current.FindFirstDescendant("checkbox_M1").AsCheckBox();

    public CheckBox CheckBoxC4 => _driver.Current.FindFirstDescendant("checkbox_C4").AsCheckBox();
    public CheckBox CheckBoxC3 => _driver.Current.FindFirstDescendant("checkbox_C3").AsCheckBox();
    public CheckBox CheckBoxC2 => _driver.Current.FindFirstDescendant("checkbox_C2").AsCheckBox();
    public CheckBox CheckBoxC1 => _driver.Current.FindFirstDescendant("checkbox_C1").AsCheckBox();

  }
}
