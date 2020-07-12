#pragma once
#include <string>
#include <iostream>

class CMenuCLI
{
public:
	void PrintMainMenu();	// Prints the main splash menu

private:

	// Strings definitions
	// Used to display menu item constants


	//Main Menu strings
	const std::string M_STR_SOFTWARETITLE		= "				PRAWN - Highway Regulation and Enforcement\n";
	const std::string M_STR_MAINMENULINE1		= "		1.	Edit Selected Devices\n";
	const std::string M_STR_MAINMENULINE2		= "		2.	Edit Database Directory\n";
	const std::string M_STR_COPYRIGHTNOTICE		= "			Copyright (c) 2020 | Ryan Hall | https://github.com/RyanHall23 \n";

	// Text formatting strings 
	const std::string M_NEWLINE = "\n";
};

