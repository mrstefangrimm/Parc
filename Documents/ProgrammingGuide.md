# Dull Programming Guide

The manual gives brief introduction to Dull. As Dull is a new invention, this is currently all information there is.

Dull is the programming language to program a Parc device. Dull is very limited an maybe it should not even be called programming language. It is called "dull" because it is very simple, simplistic, limited.

What you can do with Dull:
 - Define a sequence of keyboard key presses or Bluetooth commands
 - Define a PIN

What you cannot do:
 - Define data structures, functions, loops and may things more. 

## Program

A program is a sequence of program steps. A program starts and ends with curly braces (`{` and `}`).

A program defines ... the current program for this is overwritten.



## Commands

### Wait command (W)

Delay the next command for a given number of milliseconds.

Example: `W 1000;`

### Keycode command (UK, BK)

A single key stroke is sent with `UK` (USB Keycode) and `BK` (Bluetooth Keycode) command. The keycodes are found on the internet. I use the following sources:
[Arduino Keyboard library Keyboard.h](https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h)

[Adafruit HID Keyboard Codes](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/ble-services)

The Arduino keyboard library is used to send keycodes over USB. The Arduino library is the reference. For Bluetooth, the Adafruit documentation is the reference.

Examples:

- `UK 'A';`
- `UK 0x41`
- `BK 0x4`

`'A'` is the same as `0x41`, which is 65 in the ASCII table. 

USB and BLE do not have the same keycodes

| Character | USB                            | BLE              |
| --------- | ------------------------------ | ---------------- |
| a         | UK 0x41;                       | BK 0x04;         |
| A         | UK 0x41;<br />UK <Shift> 0x61; | BK <Shift> 0x04; |

###### Predefined keycodes

The most common keycodes have a predefined token in Dull. Instead of `UK 0xB3` or `BK 0x2B` for sending an <Tab>, `UK <Tab>` resp. `BK <Tab>` can be used.

Predefined tokens are: <Del>, <Tab>, <Enter>, <Space>

 Examples: 

- `BK <Enter>;`
- `UK <Space>;`

###### Modifiers

 A typical modifier is <Ctrl> to send a capital letter.
 Modifiers are: <Ctrl>,  <Shift>,  <Alt>,  <Win>

Examples: 

- `UK <Win> 'l';`
- `BK <Ctrl> <Alt> <Del>;`

To repeat the same keypress, use the `-r` option or modifier.

Examples: 

- `BK -r12 <Tab>;`

###### Dual Keycode

This is only available for USB. The example formats a document in Visual Studio.

Example:

- `UK <Ctrl> "k d";`

### Text Command (UT, BT)

The text command sends a array of characters. Quote the text if the text contains a space. 

Examples: 

- `UT hello;`
- `BT "hello world";`

### BleControlkey Command (BC)

A Ble control key is text command that is sent to the receiving device and is interpreted on the device and is device of program specific.
// If the device cannot handle it, it just ignores it.

[Adafruit BLEHIDCONTROLKEY](https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/ble-services)
[Free BSD USB HID Usage](http://www.freebsddiary.org/APC/usb_hid_usages.php)

Examples: 

- `BC Volume+;`
- `BC 0xE9`

`Volume+` is the same as `0xE9`. Volume+ as on the Adafruit website and 0xE9 as on the Free BSD website.





## PIN (P N)

To set the personal identification number (PIN), the syntax is different to commands, as the PIN is unique for the device.

Using a foreign keyboard



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

