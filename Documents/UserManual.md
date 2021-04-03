# Parc User Manual

You've built the device and you want to use it. This manual will help you to do so. As you will see,  it is easy and you have all the tools you need.

## Parts and Tools

- Parc device
- Serial terminal
- USB cable

You obviously need a Parc device with the installed firmware. Find out more on its instructable. Further you need a terminal emulator to communicate resp. program the device.  The Arduino Studio comes with a terminal which they call "Serial Monitor". Actually, that is all you need. Of course you will need a USB cable.

## Functions

The device has five function buttons (A - E) and two mode switches (M1, M0). The PIN switches are covered later in this document as this is an optional feature.

Having modes 0, 1, 2, 3 and buttons A, B, C, D, E, this is a total of twenty functions. I figured out that this is a ideal setup for me so that I can still remember all my twenty functions.

Here is an example: Mode 3 is my media mode.

| Mode 3 | Bluetooth commands for buttons A - E                         |
| ------ | ------------------------------------------------------------ |
|        | A: Play/pause (PLAYPAUSE)<br />B: Next (MEDIANEXT)<br />C: Volume Up (VOLUME+)<br />D: Volume Down (VOLUME-)<br />E: Mute (MUTE) |

The Bluetooth commands work on lots of media devices and media players. Please checkout the Adafruit documentation as the [FAQ](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/faq) on this topic. For example, it works with youtube except "Next", which has no effect. But here come the great thing about Parc: You can try out all the available BLE control keys on your device (search for [BLEHIDCONTROLKEY](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/ble-services) on the Adafruit website for the full list)  and then define the program behind a function key as you wish.

## Status Lights

###### Charging Light

A orange light left of the USB connector. If the battery is fully charged it is off.

###### Error Light

If it is permanently on, the memory is low. This results in a malfunction of the device. You have to re-write or skip some of your programs. As the programs are on the heap, the program for mode 3 and button E is affected first.

###### Bluetooth Light

A blue light on the front of the device. It is on when the device is connected to another device.



## Serial Ports

###### USB Port



###### Debugging Port





# Programming

Below is a list of examples. You may want to check out the [Programming Guide](./ProgrammingGuide.md) first. The text command `ut`and `bt` use most memory. Also note that the maximal possible length of a command is 30 characters.

###### Save Memory

Here is a trick to save memory. I have a program to open instructables.com and sign in. to save memory, i have the following batch file in the folder C:\Parc\ on my Windows PC:

| C:\Parc\instructables.bat                            |
| ---------------------------------------------------- |
| `start https://www.instructables.com/account/login/` |

To open instructables.com looks like this in my program:

 `{ 0 C: ...; UK <Win>; W 1000; UT c:\parc\i; W 1000; UK <Enter>; W 8000; ...`

The program presses the <Win> button, waits 1 second and then types in c:\parc\i. This is already sufficient to localized instructables.bat. Then the program waits 1 second, presses enter and waits another  8 seconds for the website to open. `c:\parc\i` uses 9 bytes only.

###### Foreign Keyboards

I am using a foreign keyboard on my Windows PC and the Parc device assumes it is an US-English keyboard. It can be mind-twisting to figure out what keycode to send to get a correct text string. Here is my simple trick: In my programs, I switch the keyboard to by a US keyboard at be begin and switch it back at the end.

`{ 0 C: UK -r2 <Win> <Space>; ... UK <Win> <Space>; }`

<Win> <Space> changes the keyboard. with the modifier `-r2`, <Win><Space> is pressed twice.



###### Example programs

I removed my personal data, otherwise this is how I programmed my Parc device

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



## PIN Security

How it works: not set, set and 0000

{ P N: 1 1 0 0 3; }

