# Parc Build Verification Test Specification

Manual test steps to test the firmware on the devices.  Unit tests and automated tests are not part of this verification test. It is expected that these tests passed.

## 0. Preparation

Required software:

- Arduino IDE
- Second terminal emulator such as YAT or PuTTY
- Atmel Flip
- build package with the simulator binaries
- ParcLib installed

Required PARC devices and Hardware:

- PARC
- PCB PARC
- UNO PARC
- Debug cable
- USB cable

## 1. Unit Tests

1. Connect the PC to the PARC device or Arduino UNO, USB cable
1. In the Arduino IDE, open the sketch in \Documents\Arduino\libraries\ParcLib\Tests\Core\BitCounterTest.
1. Upload the firmware and open the Serial Monitor
   - All tests pass
1. Repeat test steps 2 and 3 for
   - \Documents\Arduino\libraries\ParcLib\Tests\Core\HidAoTest
   - \Documents\Arduino\libraries\ParcLib\Tests\Core\KeypadAoTest
   - \Documents\Arduino\libraries\ParcLib\Tests\Core\StringParcTest
   - \Documents\Arduino\libraries\ParcLib\Tests\Core\SystemMonitorAoTest
   - \Documents\Arduino\libraries\ParcLib\Tests\Core\TerminalAoTest\TerminalAoCommandsTest
   - \Documents\Arduino\libraries\ParcLib\Tests\Core\TerminalAoTest\TerminalAoVariousTest
   - \Documents\Arduino\libraries\ParcLib\Tests\Core\ValueQueueTest
   - \Documents\Arduino\libraries\ParcLib\Tests\Feather\ProgramTest

## 2. Test PARC

5. Connect the PC to the PARC device, USB and Debug cable
1. In the Arduino IDE, open the project \Software\Src\ParcApp\ParcApp.ino 
1. Enable the logging by changing the logging from `FakeLogger`to `SoftwareSerial`
1. Upload the firmware
1. Reset the device by pressing the reset button
   - the used pins and the modes are logged on the debug output
1. In the Arduino IDE, open the Serial Monitor and send a `?`
   - The programs slots are listed, non is used
1. Open the debug monitor
   - The free memory is logged frequently
1. In the Serial Monitor,  send `{ 0 A: UK <Win>; W 2000; UT notepad; W 1000; UK <Enter>; W 1000; UT hello; } ` 
   - The program is repeated on the serial monitor and accepted with "thank you"
   - The created program steps are logged on the debug monitor
1. On the device, press the "A"-button
   - The program is executed, the notepad is opened and "hello" is written
1. Open the Windows Bluetooth setting and connect the device
1. In the Serial Monitor,  send `{ 3 E: BK <Win>; W 2000; BT notepad; W 1000; BK <Enter>; W 1000; BT hello; } ` 
    - The program is repeated on the serial monitor and accepted with "thank you"
    - The created program steps are logged on the debug monitor
1. On the device, set the mode to "3" press the "E"-button
    - The program is executed, the notepad is opened and "hello" is written
1. In the Serial Monitor,  send `{ P N: 1 1 0 1 3; } ` 
    - The pin is repeated on the serial monitor and accepted with "thank you"
    - On the debug monitor, "got pin" is logged.
1. On the device, set a wrong pin, set the mode to "0" and press the "A"-button
    - The status light is lit
    - On the debug monitor, "Wrong PIN" is logged
1. On the device, set the correct pin, set the mode to "0" and press the "A"-button
    - The program is executed, the notepad is opened and "hello" is written
1. End of the test
     - Disable the logging by changing the logging from `SoftwareSerial` to `FakeLogger`
     - Disconnect the device from Bluetooth

## 3.  Test PCB PARC

21. Connect the PC to the PCB PARC device, USB and Debug cable
1. In the Arduino IDE, open the project \Software\Src\PcbParcApp\PcbParcApp.ino 
1. Execute the test steps from "Test Parc"
1.. End of the test
    - Disable the logging by changing the logging from `SoftwareSerial` to `FakeLogger`
    - Disconnect the device from Bluetooth

## 4. Test UNO PARC

24. Connect the PC to the UNOPARC breadboard, USB and Debug cable
1. In the Arduino IDE, open the project \Software\Src\UnoParcApp\UnoParcApp.ino 
1. Upload the firmware
1. In the debug monitor, which is log output and terminal, send a `?`
    - The programs slots are listed, non is used
1. Start Atmel Flip
1. On the UNO, reset the Arduino Serial chip
1. In Flip, connect via USB
1. In Flip, load the Arduino-keyboard-0.3.hex to the UNO
1. In Flip, run the firmware update
1. Unplug and re-plug the Arduino UNO (the Arduino UNO is now acting as a USB keyboard)
1. In the debug monitor, send `{ 0 A: UK <Win>; W 2000; UK <Win>; } ` 
    - The program is repeated on the debug monitor and accepted with "thank you"
    - The created program steps are logged on the debug monitor
1. On the UNO PARC breadboard, press the "A"-button
    - The Windows opens for 2 seconds
1. End of the test
     - In Flip, revert the firmware (HEX file).

## 5. Test Simulator

37. Start the application 'WinParc.App.exe'
1. In the text field "Serial Monitor" send a `?`
    - The programs slots are listed, non is used
1. In the text field "Serial Monitor" send a `{ P N: 1 1 0 1 2; }`
    - The pin is repeated on the serial monitor and accepted with "thank you"
    - On the debug output, "got pin" is logged
1. In the text field "Serial Monitor" send a `{ 0 A: UT hello; }`
    - The program is repeated on the serial monitor and accepted with "thank you"
    - The created program step is logged on the debug monitor
1. Press the "A"-button
    - "Button press ignored." on the debug output
1. Set the code C4=true and press the "A"-button
    - "Wrong PIN, remaining retries: 2" on the debug output
    - Warn LED is "On" for 5 seconds
1. Set the code C4=true, C3=true, C1=true and press the "A"-button
    - "Execute { UT }" on the debug output
1. End of the test