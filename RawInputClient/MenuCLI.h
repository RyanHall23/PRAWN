#pragma once
#include <string>
#include <iostream>

#include <windows.h>

class CMenuCLI
{
private:

	// Strings definitions
	// Used to display menu item constants


	// Main Menu strings
	const std::string M_STR_SOFTWARETITLE		= "				PRAWN - Highway Regulation and Enforcement\n";
	const std::string M_STR_MAINMENULINE1		= "		1.	Edit Scanner Devices\n";
	const std::string M_STR_MAINMENULINE2		= "		2.	Edit Database Directory\n";
	const std::string M_STR_COPYRIGHTNOTICE		= "			Copyright (c) 2020 | Ryan Hall | https://github.com/RyanHall23 \n";

	// Edit Selected Devices Menu String
	const std::string M_STR_EDITSCANNERDEVICES1 = "		1.	Add Scanner\n";
	const std::string M_STR_EDITSCANNERDEVICES2 = "		2.	Remove Scanner\n";
	const std::string M_STR_EDITSCANNERDEVICES3 = "		3.	Change Scanner\n";
	const std::string M_STR_EDITSCANNERDEVICES4 = "		0.	Return to Main Menu\n";


	// Text formatting strings 
	const std::string M_NEWLINE = "\n";
	const std::string M_TABLINE = "\t";

public:
	void PrintMainMenu();	// Prints the main splash menu
	void PrintScannerDevicesMenu();

	void ClearMenu();

};

