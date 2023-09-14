// Copyright (c) 2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using System.Diagnostics;
using TechTalk.SpecFlow;

namespace WinParc.App.Specs.Hooks {

  [Binding]
  public static class TestRunHook {

    [AfterTestRun]
    public static void KillCppProcess() {
      foreach (var process in Process.GetProcessesByName("WinParc.App")) {
        process.Kill();
      }
    }
  }
}