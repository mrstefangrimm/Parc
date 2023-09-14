# Copyright (c) 2023 Stefan Grimm. All rights reserved.
# Licensed under the LGPL. See LICENSE file in the project root for full license information.
#
Feature: Executing
Executes programs

Scenario: Program "hello" is executed if added to mode 0, button A and no PIN defined and no Code set
	Given the program hello exists
	When the button "A" is pressed
	Then the program is executed

Scenario: Program "hello" is not executed if added to mode 0, button A and no PIN defined and no Code set
	Given the program hello exists
	When the button "B" is pressed
	Then the program is not executed

Scenario: Program "hello" is executed if added to mode 0, button A and no PIN defined and no Code is 1 1 0 1
	Given the program hello exists
	And Code C4=1, C3=1, C2=0, C1=1
	When the button "A" is pressed
	Then the program is executed

Scenario: Program "hello" is not executed if added to mode 0, button A and no PIN defined and mode is 1
	Given the program hello exists
	And Mode M1=0, M0=1
	When the button "A" is pressed
	Then the program is not executed

Scenario: Program "hello" is not executed if added to mode 0, button A and PIN defined
	Given the program hello exists
	And PIN is set
	When the button "A" is pressed
	Then the program is not executed

Scenario: Program "hello" is not executed if added to mode 0, button A and PIN defined and code is wrong
	Given the program hello exists
	And PIN is set
	And Code C4=1, C3=0, C2=0, C1=1
	When the button "A" is pressed
	Then the program is not executed

Scenario: Program "hello" is executed if added to mode 0, button A and PIN defined and code is correct
	Given the program hello exists
	And PIN is set
	And Code C4=1, C3=1, C2=0, C1=1
	When the button "A" is pressed
	Then the program is executed
