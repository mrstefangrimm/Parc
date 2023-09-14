// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using OpenQA.Selenium.Appium.Windows;
using SpecFlow.Actions.WindowsAppDriver;

namespace WinParc.App.Specs.SimulatorApp {

  internal class ParcSimulatorElements {

    protected readonly AppDriver _appDriver;

    public ParcSimulatorElements(AppDriver appDriver) {
      _appDriver = appDriver;
    }

    public WindowsElement SendText => _appDriver.Current.FindElementByAccessibilityId("textbox_SendText");

    public WindowsElement SendPressCmd => _appDriver.Current.FindElementByAccessibilityId("button_SendPressCmd");

    public WindowsElement TerminalOutput => _appDriver.Current.FindElementByAccessibilityId("textbox_TerminalOutput");

    public WindowsElement DebugOutput => _appDriver.Current.FindElementByAccessibilityId("textbox_DebugOutput");

    public WindowsElement ButtonA => _appDriver.Current.FindElementByAccessibilityId("button_A");
    public WindowsElement ButtonB => _appDriver.Current.FindElementByAccessibilityId("button_B");
    public WindowsElement ButtonC => _appDriver.Current.FindElementByAccessibilityId("button_C");
    public WindowsElement ButtonD => _appDriver.Current.FindElementByAccessibilityId("button_D");
    public WindowsElement ButtonE => _appDriver.Current.FindElementByAccessibilityId("button_E");

    public WindowsElement CheckBoxM0 => _appDriver.Current.FindElementByAccessibilityId("checkbox_M0");
    public WindowsElement CheckBoxM1 => _appDriver.Current.FindElementByAccessibilityId("checkbox_M1");

    public WindowsElement CheckBoxC4 => _appDriver.Current.FindElementByAccessibilityId("checkbox_C4");
    public WindowsElement CheckBoxC3 => _appDriver.Current.FindElementByAccessibilityId("checkbox_C3");
    public WindowsElement CheckBoxC2 => _appDriver.Current.FindElementByAccessibilityId("checkbox_C2");
    public WindowsElement CheckBoxC1 => _appDriver.Current.FindElementByAccessibilityId("checkbox_C1");

  }
}
