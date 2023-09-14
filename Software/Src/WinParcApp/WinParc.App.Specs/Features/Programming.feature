# Copyright (c) 2023 Stefan Grimm. All rights reserved.
# Licensed under the LGPL. See LICENSE file in the project root for full license information.
#
Feature: Programming
Program the device with programs and PIN

Scenario: Check uninitialized
	Given there are no programs or pin
	When the command "?" is sent
	Then the output is a list of empty program slots

Scenario: Add PIN 1 1 0 1 with 2 retries
	Given there are no programs or pin 
	When the command "{ P N: 1 1 0 1 2; }" is sent
	Then the command is accepted on the output with "thank you" and on the debug output with "got pin"

Scenario: Add program "hello" to mode 0, button A
	Given there are no programs or pin
	When the command "{ 0 A: UT hello; }" is sent
	Then the command is accepted on the output with "thank you" and on the debug output with "Create UsbText_t"
	And the program slot 0 is occupied