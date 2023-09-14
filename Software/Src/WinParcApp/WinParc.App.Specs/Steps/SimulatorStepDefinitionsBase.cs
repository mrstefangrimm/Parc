// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using System.Diagnostics;
using TechTalk.SpecFlow;
using WinParc.App.Specs.SimulatorApp;

namespace WinParc.App.Specs.Steps {

  [Binding]
  public class SimulatorStepDefinitionsBase {

    protected readonly ParcSimulator _simulator;

    public SimulatorStepDefinitionsBase(ParcSimulator simulator) {
      _simulator = simulator;
    }

    [AfterTestRun]
    public static void KillCppProcess() {
      foreach (var process in Process.GetProcessesByName("WinParc.App")) {
        process.Kill();
      }
    }
  }
}