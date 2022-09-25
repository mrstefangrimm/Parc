// Copyright (c) 2021-2022 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using System.Windows;

namespace WinParc.View {
  /// <summary>
  /// Interaction logic for App.xaml
  /// </summary>
  public partial class App : Application {
    private void Application_Startup(object sender, StartupEventArgs e) {
      var mainWindow = new MainWindow();
      mainWindow.DataContext = new MainWindowViewModel(new Core.Status());
      mainWindow.Show();
    }
  }
}
