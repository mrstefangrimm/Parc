// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using FluentAssertions;
using System.Threading;
using TechTalk.SpecFlow;
using WinParc.App.Specs.SimulatorApp;

namespace WinParc.App.Specs.Steps {

  [Binding]
  public sealed class ExecutingStepDefinitions : SimulatorStepDefinitionsBase {

    public ExecutingStepDefinitions(ParcSimulator simulator) : base(simulator) { }

    [Given(@"the program hello exists")]
    public void GivenTheProgramHelloExists() {
      _simulator.SendCommand("{ 0 A: UT hello; }");
    }

    [Given(@"PIN is set")]
    public void GivenPINIsSet() {
      _simulator.SendCommand("{ P N: 1 1 0 1 2; }");
    }

    [Given(@"Mode M1=(.*), M0=(.*)")]
    public void GivenModeM1M0(int m1, int m0) {
      _simulator.SetMode(m1 > 0, m0 > 0);
    }

    [Given(@"Code C4=(.*), C3=(.*), C2=(.*), C1=(.*)")]
    public void GivenCodeC4C3C2C1(int c4, int c3, int c2, int c1) {
      _simulator.SetCode(c4 > 0, c3 > 0, c2 > 0, c1 > 0);
    }

    [When(@"the button ""([^""]*)"" is pressed")]
    public void WhenTheButtonIsPressed(string button) {
      Thread.Sleep(500);
      _simulator.PressButton(button);
    }

    [Then(@"the program is executed")]
    public void ThenTheProgramIsExecuted() {
      Thread.Sleep(800);
      _simulator.GetDebugOutput().Should().Contain("Execute { UT }");
    }

    [Then(@"the program is not executed")]
    public void ThenTheProgramIsNotExecuted() {
      _simulator.GetDebugOutput().Should().NotContain("Execute { UT }");
    }

  }
}
