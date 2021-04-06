# Parc User Manual

You've built the device and you want to use it. This manual will help you to do so. As you will see,  it is easy and you have all the tools you need.

## Parts and Tools

- Parc device
- Serial terminal
- USB cable

You obviously need a Parc device with the installed firmware. Find out more on its instructable. Further you need a terminal emulator to communicate resp. program the device.  The Arduino Studio comes with a terminal which they call "Serial Monitor". Actually, that is all you need. Of course you will need a USB cable.

## Functions

The device has five function buttons (A - E) and two mode switches (M1, M0). The PIN switches are covered later in this document as this is an optional feature.

Modes 0, 1, 2, 3 multiplied with buttons A, B, C, D, E makes a total of twenty functions. I figured out that this setup with four modes and  five function buttons is ideal for me to remember all my twenty functions.

Here is an example: Mode 3 is my media mode.

| Mode 3 | Bluetooth commands for buttons A - E                         |
| ------ | ------------------------------------------------------------ |
|        | A: Play/pause (PLAYPAUSE)<br />B: Next (MEDIANEXT)<br />C: Volume Up (VOLUME+)<br />D: Volume Down (VOLUME-)<br />E: Mute (MUTE) |

The Bluetooth commands work on lots of media devices and media apps (youtube, Soundcloud). Please have a look at the Adafruit [FAQ](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/faq) on this topic. For example, the commands in the table above work with youtube except for "Next", which has no effect. But here come the great thing about Parc: You can try out all the available BLE control keys on your device (search for [BLEHIDCONTROLKEY](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/ble-services) on the Adafruit website for the full list)  and then define the program behind a function key as you wish.

## Status Lights

###### Charging Light

A orange light left of the USB connector. If the battery is fully charged it is off.

###### Warning Light

If it is permanently on, the memory is low. This results in a malfunction of the device. You have to re-write or skip some of your programs. As the programs are on the heap, the program for mode 3 and button E is affected first.

###### Bluetooth Light

A blue light on the front of the device. It is on when the device is connected to another device.

## Serial Ports

###### USB Port

The USB port acts as a HID device when a program is executed. For the PC or Smartphone or Tablet, the Parc device is a keyboard.

At the same time, and that is really great, a Terminal emulator can be connected to the associated COM port to program the device

###### Debugging Port

To use the debugging port, you need a special USB cable and you have to enable the logging in the Arduino program. It is not in the scope of this document, but if you've used Arduino's SoftwareSerial library before, this should not be a problem for you.

# Programming

At the end of this section is a list with examples. You may want to have a look at the [Programming Guide](./ProgrammingGuide.md) too. To program the Parc remote control, you connect the device with an USB cable to the PC and connect with a Terminal emulator to the COM port.

To write your first program, type in: `{ 0 A: UT "hello world"; }`. Open the notepad text editor and press the "A"  key on the Parc remote control with both mode switches in zero position; voilà. Not yet impressed? How about a program that opens the text editor first? Try: `{ 0 A: UK <Win>; W 1000; UT notepad; W 2000; UK <Enter>; W 1000; UT "hello world"; }` .

To modify a program, just write a new one and the old one is deleted. Lets say for mode/button 0/C you wrote: `{ 0 C: UT "hello wordl"; }`. Fix it by writing: `{ 0 C: UT "hello world"; }`. 

To delete a program, write a program without a program step (`{ 0 C: }`).

The maximal possible length of a command is 30 characters. This is the longest possible command: `{ 0 C: UT "1234567890123456789012345"; }`, You can always increase the number in the Arduino program, but the memory is limited and you should avoid long text commands if possible.  

Typing in a program in a Terminal emulator is cumbersome. Instead, you can write the programs in a text editor and copy-paste the code to the Terminal.

###### Strategy to persist your programs

The programs on the Parc remote control contain passwords (mine do). On the device, the programs are in the volatile memory. But a plain text file on the PC with all the programs and passwords is not a good idea. Here is my trick: Mange the programs in a password manager. The entries in a password manager usually have field for free text.

###### Strategy to Save Memory

The text command `UT`and `BT` use most memory. Here is a trick to save memory: Use batch files and minimize the search strings.

Example: The program opens instructables.com and signs in. A batch file in the folder C:\Parc\ opens the web browser and navigates to the web site:

| C:\Parc\instructables.bat                            |
| ---------------------------------------------------- |
| `start https://www.instructables.com/account/login/` |

Open instructables.com in the Dull program looks like this:

 `{ 0 C: ...; UK <Win>; W 1000; UT c:\parc\i; W 1000; UK <Enter>; W 8000; ...`

The program presses the `<Win>` button, waits 1 second and then types in c:\parc\i. This is already sufficient to localized instructables.bat. Then the program waits 1 second, presses Enter and waits another  8 seconds for the website to open. `c:\parc\i` uses only 9 bytes.

###### Foreign Keyboards

I am using a foreign keyboard on my Windows PC and the Parc device assumes it is an US-English keyboard. It can be mind-twisting to figure out what keycode to send to get a correct text string. Here is my simple trick: In my programs, I switch the keyboard to by a US keyboard at be begin and switch it back at the end.

`{ 0 C: UK -r2 <Win> <Space>; ... UK <Win> <Space>; }`

`<Win> <Space>` changes the keyboard. with the "-r" modifier, `-r2 <Win><Space>`, it is pressed twice.

###### Example programs

I removed my personal data, otherwise this is how my Parc remote control is currently programmed:

| Name     | Mode / Button | Program                                                      |
| -------- | ------------- | ------------------------------------------------------------ |
| Home USB | 0 / A         | `{ 0 A: UK <Ctrl> <Alt> <Del>; W 2000; UT password; W 10000; UK <Win> <Space>; }` |
| | 0 / B | `{ 0 B: UK <Win> 'l'; }` |
| Home BLE | 1 / A |`{ 1 A: BK <Ctrl> <Alt> <Del>; W 2000; BT password; W 1000; BK <Enter>; W 10000; BK <Win> <Space>; }` |
| | 1 / B | `{ 1 B: BK <Win> 0xF; }` |
| | 1 / C | `{ 1 C: BK -r2 <Win> <Space>; W 1000; BK <Win>; W 1000; BT pwd-mgr; W 2000; BK <Enter>; W 5000; BT password; BK -r6 <Tab>; W 1000; BK <Enter>; BK <Win> <Space>; }` |
| | 1 / D | `{ 1 D: BK -r2 <Win> <Space>; BK <Win>; W 1000; BT c:\parc\z; W 1000; BK <Enter>; W 8000; BK -r4 <Tab>; BT loginname; W 1000; BK <Tab>; BT password; BK <Win> <Space>; }` |
| Work | 2 / A | `{ 2 A: UK <Ctrl> <Alt> <Del>; W 5000; UT password; }` |
| | 2 / B | `{ 2 B: UK <Win> 'l'; }` |
| | 2 / C | `{ 2 C: UK <Win>; W 1000; UT time-mgmt-tool; W 2000; UK <Enter>; W 5000; UT login-name; UK <Tab>; UT password; UK <Tab>; W 5000; UK <Enter>; }` |
| | 2 / D |`{ 2 D: UK <Win>; W 1000; UT "Work Tasks Website"; W 1000; UK <Enter>; }` |
| | 2 / E | `{ 2 E: UK <Win>; W 1000; UT VPN-Client; W 1000; UK <Enter>; W 1000; UK <Win>; W 1000; UT "Key Gen"; W 1000; UK <Enter>; UK <Tab>; UK <Space>; }` |
| Media | 3 / A | `{ 3 A: BC PlayPause; }` |
| | 3 / B | `{ 3 B: BC MediaNext; }` |
| | 3 / C | `{ 3 C: BC Volume+; }` |
| | 3 / D | `{ 3 D: BC Volume-; }` |
| | 3 / E | `{ 3 E: BC Mute 1000; }` |



## Security

The PIN is optional. Once you have set a PIN it is no longer optional. If your programs contain passwords you should define a PIN, so you have some security if you loose your device. If the "attacker" presses the function buttons and the PIN is incorrect, the device goes into error state. And it is not possible to unlock the device after that. A hardware reset is required and this deletes all the programs and the current PIN.

If you've set a PIN and the code switches are all set to zero, keypresses are ignored.

Here is how you set a PIN: `{ P N: 1 0 1 0 2; }`. The last number is the number of retries before the device locks.



<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.