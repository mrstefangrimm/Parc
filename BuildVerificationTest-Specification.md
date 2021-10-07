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

## 1. Test PARC

1. Connect the PC to the PARC device, USB and Debug cable
2. In the Arduino IDE, open the project \Software\Src\ParcApp\ParcApp.ino 
3. Enable the logging by changing the logging from `FakeLogger`to `SoftwareSerial`
4. Upload the firmware
5. In the Arduino IDE, open the Serial Monitor and send a `?`
   - The programs slots are listed, non is used
6. Open the debug monitor
   - The free memory is logged frequently
7. In the Serial Monitor,  send `{ 0 A: UK <Win>; W 2000; UT notepad; W 1000; UK <Enter>; W 1000; UT hello; } ` 
   - The program is repeated on the serial monitor and accepted with "thank you"
   - The created program steps are logged on the debug monitor
8. On the device, press the "A"-button
   - The program is executed, the notepad is opened and "hello" is written
9. Open the Windows Bluetooth setting and connect the device
10.  In the Serial Monitor,  send `{ 3 E: BK <Win>; W 2000; BT notepad; W 1000; BK <Enter>; W 1000; BT hello; } ` 
    - The program is repeated on the serial monitor and accepted with "thank you"
    - The created program steps are logged on the debug monitor
11. On the device, set the mode to "3" press the "E"-button
    - The program is executed, the notepad is opened and "hello" is written
12. In the Serial Monitor,  send `{ P N: 1 1 0 1 3; } ` 
    - The pin is repeated on the serial monitor and accepted with "thank you"
    - On the debug monitor, "got pin" is logged.
13. On the device, set a wrong pin, set the mode to "0" and press the "A"-button
    - The status light is lit
    - On the debug monitor, "Wrong pin" is logged
14. On the device, set the correct pin, set the mode to "0" and press the "A"-button
    - The program is executed, the notepad is opened and "hello" is written
15. End of the test

## 2.  Test PCB PARC

16. Connect the PC to the PCB PARC device, USB and Debug cable
17. In the Arduino IDE, open the project \Software\Src\PcbParcApp\PcbParcApp.ino 
18. Execute the test steps from "Test Parc"
19. End of the test

## 3. Test UNO PARC

20. Connect the PC to the UNOPARC breadboard, USB and Debug cable
21. In the Arduino IDE, open the project \Software\Src\UnoParcApp\UnoParcApp.ino 
22. Enable the logging by changing the logging from `FakeLogger`to `SoftwareSerial`
23. Upload the firmware
24. In the debug monitor, which is log output and terminal, send a `?`
    - The programs slots are listed, non is used
25. Start Atmel Flip
26. On the UNO, reset the Arduino Serial chip
27. In Flip, connect via USB
28. In Flip, load the Arduino-keyboard-0.3.hex to the UNO
29. In Flip, run the firmware update
30. Unplug and re-plug the Arduino UNO
31. In the debug monitor, send `{ 0 A: UK <Win>; W 2000; UK <Win> } ` 
    - The program is repeated on the debug monitor and accepted with "thank you"
    - The created program steps are logged on the debug monitor
32. On the UNO PARC breadboard, press the "A"-button
    - The Windows opens for 2 seconds

## 4. Test Simulator

33. Start the application 'WinParc.App.exe'
34. In the text field "Serial Monitor" send a `?`
    - The programs slots are listed, non is used
35. In the text field "Serial Monitor" send a `{ P N: 1 1 0 1 2; }`
    - The pin is repeated on the serial monitor and accepted with "thank you"
    - On the debug output, "got pin" is logged
36. In the text field "Serial Monitor" send a `{ 0 A: UT hello; }`
    - The program is repeated on the serial monitor and accepted with "thank you"
    - The created program step is logged on the debug monitor
37. Press the "A"-button
    - "Button press ignored." on the debug output
38. Set the code C4=true and press the "A"-button
    - "Wrong PIN, remaining retries: 2" on the debug output
39. Set the code C4=true, C3=true, C1=true and press the "A"-button
    - "Execute { UT }" on the debug output
40. End of the test