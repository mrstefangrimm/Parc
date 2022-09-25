// Copyright (c) 2021 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Input;
using WinParc.Core;

namespace WinParc.View {

  public class MainWindowViewModel : INotifyPropertyChanged {

    private Status _status;
    private ICommand _btnPressCmd;
    private ICommand _sendPressCmd;
    private bool _isCheckedM0;
    private bool _isCheckedM1;
    private bool _isCheckedC1;
    private bool _isCheckedC2;
    private bool _isCheckedC3;
    private bool _isCheckedC4;
    private string _sendText;
    private string _debugOutput;
    private string _terminalOutput;
    private string _warnStatus;

    public MainWindowViewModel(Status status) {
      _status = status;
      _btnPressCmd = new RelayCommand<Button>(param => {
        _status.SetKeyPressed(param, true);
      });
      _sendPressCmd = new RelayCommand<object>(param => {
        var text = _sendText;
        if (!string.IsNullOrEmpty(text)) {
          SendText = null;
          _status.SetTerminalCommand(text);
        }
      });

      _status.DebugOutputChanged += (sender, args) => { DebugOutput += args.Message; };
      _status.TerminalOutputChanged += (sender, args) => { TerminalOutput += args.Message; };
      _status.WarnStatusChanged += (sender, args) => {
        if (args.Warn) {
          WarnLedStatus = "On";
        }
        else {
          WarnLedStatus = "Off";
        }
      };
    }

    public event PropertyChangedEventHandler PropertyChanged;

    public bool IsCheckedM0 {
      get { return _isCheckedM0; }
      set {
        if (_isCheckedM0 != value) {
          _isCheckedM0 = value;
          _status.SetKeyPressed(Button.M0, value);
          OnPropertyChanged();
        }
      }
    }

    public bool IsCheckedM1 {
      get { return _isCheckedM1; }
      set {
        if (_isCheckedM1 != value) {
          _isCheckedM1 = value;
          _status.SetKeyPressed(Button.M1, value);
          OnPropertyChanged();
        }
      }
    }

    public bool IsCheckedC1 {
      get { return _isCheckedC1; }
      set {
        if (_isCheckedC1 != value) {
          _isCheckedC1 = value;
          _status.SetKeyPressed(Button.C1, value);
          OnPropertyChanged();
        }
      }
    }

    public bool IsCheckedC2 {
      get { return _isCheckedC2; }
      set {
        if (_isCheckedC2 != value) {
          _isCheckedC2 = value;
          _status.SetKeyPressed(Button.C2, value);
          OnPropertyChanged();
        }
      }
    }

    public bool IsCheckedC3 {
      get { return _isCheckedC3; }
      set {
        if (_isCheckedC3 != value) {
          _isCheckedC3 = value;
          _status.SetKeyPressed(Button.C3, value);
          OnPropertyChanged();
        }
      }
    }

    public bool IsCheckedC4 {
      get { return _isCheckedC4; }
      set {
        if (_isCheckedC4 != value) {
          _isCheckedC4 = value;
          _status.SetKeyPressed(Button.C4, value);
          OnPropertyChanged();
        }
      }
    }

    public string WarnLedStatus {
      get {
        return _warnStatus;
      }
      set {
        if (value != _warnStatus) {
          _warnStatus = value;
          OnPropertyChanged();
        }
      }
    }

    public string SendText {
      get { return _sendText; }
      set {
        if (_sendText != value) {
          _sendText = value;
          OnPropertyChanged();
        }
      }
    }

    public string TerminalOutput {
      get { return _terminalOutput; }
      set {
        if (_terminalOutput != value) {
          _terminalOutput = value;
          OnPropertyChanged();
        }
      }
    }

    public string DebugOutput {
      get { return _debugOutput; }
      set {
        if (_debugOutput != value) {
          _debugOutput = value;
          OnPropertyChanged();
        }
      }
    }

    public ICommand ButtonPressCmd {
      get {
        return _btnPressCmd;
      }
    }

    public ICommand SendPressCmd {
      get {
        return _sendPressCmd;
      }
    }

    protected void OnPropertyChanged([CallerMemberName] string propertyName = null) {
      PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }

  }
}
