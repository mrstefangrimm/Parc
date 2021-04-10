# Dull Programming Guide

The manual gives brief introduction to Dull. As Dull is a new invention, this is currently all information there is.

Dull is the programming language to program a Parc remote control. Dull is very limited an maybe it should not even be called programming language. It is called "dull" because it is very simple, simplistic, limited.

What you can do with Dull:
 - Define a sequence of keyboard key presses or Bluetooth commands
 - Define a PIN

What you cannot do:
 - Define data structures, functions, loops and may things more. 

## Program

A program is a sequence of program steps. A program starts and ends with curly braces (`{` ... `}`). The opening brace is followed by the mode and the function key the program is for. The program steps are delimited by semicolons.

Example of a program that writes Hello World on two lines when button A is pressed: `{ 0 A: UT Hello; UK <Enter>; UT World; }`

## Commands

### Wait command (W)

Delay the next command for a given number of milliseconds.

Example: `W 1000;`

### Keycode command (UK, BK)

A single key stroke is sent with `UK` (**U**SB **K**eycode) and `BK` (**B**luetooth **K**eycode) command. The keycodes are found on the internet:

- [Arduino Keyboard library Keyboard.h](https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h)
- [Adafruit HID Keyboard Codes](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/ble-services)

The Arduino keyboard library is used to send keycodes over USB. Therefore the Arduino library is the reference. For Bluetooth, the Adafruit documentation is the reference.

Examples:

- `UK 'A';`
- `UK 0x41;`
- `BK <Shift> 0x4;`

`'A'` is the same as `0x41`, which is 65 in the ASCII table. 

USB and BLE do not have the same keycodes

| Character | USB                              | BLE               |
| --------- | -------------------------------- | ----------------- |
| a         | `UK 0x41`                        | `BK 0x04`         |
| A         | `UK 0x41`<br />`UK <Shift> 0x61` | `BK <Shift> 0x04` |

###### Predefined keycodes

The most common keycodes have a predefined token in Dull. Instead of `UK 0xB3` or `BK 0x2B` for sending an Tab-character, `UK <Tab>` resp. `BK <Tab>` can be used.

Predefined tokens are: `<Del>`, `<Tab>`, `<Enter>`, `<Space>`

 Examples: 

- `BK <Enter>;`
- `UK <Space>;`

###### Modifiers

 A typical modifier is `<Shift>` to send a capital letter.
 Modifiers are: `<Ctrl>`,  `<Shift>`,  `<Alt>`,  `<Win>`

Examples: 

- `UK <Win> 'l';`
- `BK <Ctrl> <Alt> <Del>;`

To repeat the same keypress, use the `-r` option or modifier.

Examples: 

- `BK -r12 <Tab>;`

###### Dual Keycode

This is only available for USB. The example below formats a document in Visual Studio.

Example:

- `UK <Ctrl> "k d";`

### Text Command (UT, BT)

The text command sends a array of characters. Quote the text if the text contains a space. 

Examples: 

- `UT hello;`
- `BT "hello world";`

### BleControlkey Command (BC)

A Ble control key is text command that is sent to the receiving device (e.g. a Smartphone or your PC) and is interpreted there. If the receiving device or program does not know the command, nothing is executed. `BC HOME` for example presses the Home button on  a Smartphone but is ignored on the PC.

Two internet sources are helpful:

- [Adafruit BLEHIDCONTROLKEY](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/ble-services)
- [Free BSD USB HID Usage](http://www.freebsddiary.org/APC/usb_hid_usages.php)

Examples: 

- `BC Volume+;`
- `BC 0xE9;`

`Volume+` is the same as `0xE9`. The term "Volume+" is from the Adafruit website and "0xE9" from the Free BSD website.



## PIN (P N)

To set the personal identification number (PIN), the syntax is different to commands, as the PIN is unique for the device. The PIN is a 4-bit number and it is possible to set the number retries (0 - 3).

Example with two retries): `{ P N: 1 0 1 0 2}` 

## Help (?)

The help lists all the 20 available program slots and if they are in use or not.

Example: `?`



## Backus Naur Form (BNF)

I tried to follow as described on [Wikipedia](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form). Example: `<opt-whitespace> ::= " " <opt-whitespace> | ""`

```BNF
<Program> ::= '{' <Address> : <Commands> '}'
<Address> ::= <Mode> ' ' <Button>
<Mode> ::= 0 | 1 | 2 | 3 
<Button> ::= A | B | C | D | E
<Commands> ::= '' | ';' | <Command> ';' | <Command> ';' <Commands> ';'
<Command> ::= <WaitCmd> | <UsbKeycodeCmd> | <UsbTextCmd> | <BleKeycodeCmd> | <BleTextCmd> | <BleControlkeyCmd>
<WaitCmd> ::= W ' ' <Duration>
<UsbKeycodeCmd> ::= <UsbKeycodeCmdSingle> | <UsbKeycodeCmdRepeated> | <UsbKeycodeCmdTwoKeysAtOnce>
<UsbKeycodeCmdSingle> ::= UK ' ' <Modifiers> ' ' <UsbKeycode> | UK ' ' <Modifier> | UK ' ' <UsbKeycode>
<UsbKeycodeCmdRepeated> ::= UK ' ' -r <Digit> ' ' <Modifiers> ' ' <UsbKeycode> ' ' | UK ' ' -r <Digit> ' '<Modifiers> | UK ' ' -r <Digit> ' '<UsbKeycode>
<UsbKeycodeCmdTwoKeysAtOnce> ::= UK ' ' <Modifiers> ' ' '"' <UsbKeycode> ' ' <Char> '"'
<UsbTextCmd> ::= UT ' ' <Text>
<BleKeycodeCmd> ::= <BleKeycodeCmdSingle> | <BleKeycodeCmdRepeated>
<BleKeycodeCmdSingle> ::= BK ' ' <Modifiers> ' ' <BleKeycode> | BK ' ' <Modifier> | BK ' ' <BleKeycode>
<BleKeycodeCmdRepeated> ::= BK ' ' -r <Digit> ' ' <Modifiers> ' ' <BleKeycode> | BK ' ' -r ' ' <Digit> ' ' <Modifiers> | BK ' ' -r <Digit> ' ' <BleKeycode>
<BleTextCmd> ::= BT ' ' <Text>
<BleControlkeyCmd> ::= <Hex8> | <Hex8> ' ' <Duation> | <Chars> | <Chars> ' ' <Duration>
<Modifiers> ::= <Modifier> | <<Modifier> > ' ' <Modifiers>
<Modifier> ::= '<Ctrl>' | '<Shift>' | '<Alt>' | '<Win>'
<UsbKeycode> ::= '<Enter>' | '<Del>' | '<Space>' | <Tab> | <8-bit-Number> | ''' <Char> '''
<BleKeycode> ::= '<Enter>' | '<Del>' | '<Space>' | <Tab> | <8-bit-Number>
<8-bit-Number> ::= [0x00..0xFF]
<Text> ::= <Chars> | '"' <SpacedChars> '"'
<SpacedChars> ::= <Chars> | <Chars> ' ' <Chars> | <SpacedChars>
<Chars> ::= <Char> | <Char> <Chars>
<Char> ::= [a..z] | [A..Z] | ' '
<Hex8> ::= [0x00..[0xFF]
<Duation> ::= [0..65535]
<Digit> ::= [0..9]

<Pin> ::= '{' P ' ' N : <Binary> ' ' <Binary> ' ' <Binary> ' ' <Binary> ' ' <Retries> '}'
<Binary> ::= 0 ¦ 1
<Retries> ::= [0..3]

<Constant> ::= '{' C ' ' <ConstantIndex> : <Text> '}'
<ConstantIndex> ::= [0..7]
```



<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.