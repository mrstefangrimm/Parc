// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using FluentAssertions;
using System.Threading;
using TechTalk.SpecFlow;
using WinParc.App.Specs.SimulatorApp;
using WinParc.App.Specs.Steps;

namespace SpecFlowCalculator.Specs.Steps {

  [Binding]
  public sealed class ProgrammingStepDefinitions : SimulatorStepDefinitionsBase {

    public ProgrammingStepDefinitions(ParcSimulator simulator) : base(simulator) { }

    [Given(@"there are no programs or pin")]
    public void GivenThereAreNoProgramsOrPin() {
    }

    [Then(@"the output is a list of empty program slots")]
    public void ThenTheOutputIsAListOfEmptyProgramSlots() {
      var output = _simulator.GetTerminalOutput();
      output.Should().NotContain("Yes");
    }

    [Then(@"the command is accepted on the output with ""([^""]*)"" and on the debug output with ""([^""]*)""")]
    public void ThenTheCommandIsAcceptedOnTheOutputWithAndOnTheDebugOutputWith(string output, string debugOutput) {
      Thread.Sleep(500);

      _simulator.GetTerminalOutput().Should().Contain(output);
      _simulator.GetDebugOutput().Should().Contain(debugOutput);
    }

    [When(@"the command ""([^""]*)"" is sent")]
    public void WhenTheCommandIsSent(string command) {
      _simulator.SendCommand(command);
    }

    [Then(@"the program slot (.*) is occupied")]
    public void ThenTheProgramSlotIsOccupied(int mode) {
      _simulator.SendCommand("?");
      Thread.Sleep(500);

      string searchPattern = "A : Yes";
      _simulator.GetTerminalOutput().Should().Contain(searchPattern);
    }
  }
}