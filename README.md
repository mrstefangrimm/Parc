[![CI-Build](https://github.com/mrstefangrimm/Parc/actions/workflows/ci-build.yml/badge.svg)](https://github.com/mrstefangrimm/Parc/actions/workflows/ci-build.yml)
[![Compile Sketches](https://github.com/mrstefangrimm/Parc/actions/workflows/compile-sketches.yml/badge.svg)](https://github.com/mrstefangrimm/Parc/actions/workflows/compile-sketches.yml)

# Parc
Parc stand for Programmable Arduino Remote Control. This repository contains the software, the hardware and the mechanical design files.

| Parc (original)        | Parc (PCB)                    |
| ---------------------- | ----------------------------- |
| ![Parc](./parc-bw.JPG) | ![PCB Parc](./pcbparc-bw.jpg) |



## Motivation

One way to automated tasks on your PC is to write scripts. To run a script you have to be logged in to your PC already. If you write a script to lets say  sign in on a web application, it is not very save to put the password directly into the script.

Parc is a microcontroller based device that acts as a HID, these problems are solved.



## Getting Started

To use the software, you will have to build a Parc device. How to build one is described in an [instructable](https://www.instructables.com/Programmable-Arduino-Remote-Control/).

The software is very flexible and it is also possible to use it together with a breadboard or an Arduino UNO.

Minimal requirements to use the software on a breadboard:

- Arduino IDE
- Adafruit feather 32u4 BLE or Arduino UNO
- MCP23008
- Breadboard
- Push buttons and toggle switches



## Examples

A user manual and a programming guide can be found in the [Parc Wiki](https://github.com/mrstefangrimm/Parc/wiki).



## Tests

Unit tests cover the functionality of the domain model classes. Some are part of the ParcLib software package. The WinParcApp Visual Studio solution contains all the unit tests. These for programming convenience and because the software architecture allows it.



## Software Toolchain

| Application | License |
| ----------- | ------- |
| ParcApp     | LGPL    |
| PcbParcApp  | LGPL    |
| UnoParcApp  | LGPL    |
| WinParcApp  | LGPL    |

| Tool                         | License                                                      |
| ---------------------------- | ------------------------------------------------------------ |
| Arduino IDE                  | GPL                                                          |
| Adafruit AVR Boards          | [Learn more]([Arduino IDE Setup \| Adafruit Feather 32u4 Bluefruit LE \| Adafruit Learning System](https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/setup)) |
| avrdude                      | GPL                                                          |
| gcc                          | GPL                                                          |
| Visual Studio Community 2022 | Microsoft license                                            |

Libraries

| Library                           | Licence           |
| :-------------------------------- | ----------------- |
| ParcLib                           | LGPL              |
| Adafruit MCP23017 Arduino Library | BSD               |
| Adafruit BluefruitLE nRF51        | BSD               |
| Arduino.h                         | LGPL              |
| ArduinoUnit                       | MIT               |
| StandardCplusplus                 | LGPL              |
| TimerOne                          | CC BY 3.0 US      |
| TemplateStateMachine              | Apache License    |
| .NET 4.8.1                        | Microsoft license |



## License

This is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.



## Project History

The idea for the project came in mid January 2021 and was first published on instructables.com in mid April 2021.



## Change History

See [github releases](https://github.com/mrstefangrimm/Parc/releases).



<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
