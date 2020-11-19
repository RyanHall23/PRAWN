#pragma once
#include <string>
#include <iostream>

#include <windows.h>

#include "Persistence.h"

class CMenuCLI
{
private:

	// Strings definitions
	// Used to display menu item constants


	// Main Menu strings
	const std::string M_STR_SOFTWARETITLE		= "				PRAWN - Highway Regulation and Enforcement\n";
	const std::string M_STR_MAINMENULINE1		= "		1.	Edit Scanner Devices\n";
	const std::string M_STR_MAINMENULINE2		= "		2.	Edit Speed and Location Properties\n";
	const std::string M_STR_MAINMENULINE3		= "		3.	Edit Database Directory\n";
	const std::string M_STR_COPYRIGHTNOTICE		= "			Copyright (c) 2020 | Ryan Hall | https://github.com/RyanHall23 \n";

	// Edit Selected Devices Menu String
	const std::string M_STR_EDITSCANNERDEVICES1 = "		1.	Add Scanner\n";
	const std::string M_STR_EDITSCANNERDEVICES2 = "		2.	Remove Scanner\n";
	const std::string M_STR_EDITSCANNERDEVICES3 = "		3.	Change Scanner\n";
	const std::string M_STR_EDITSCANNERDEVICES4 = "		0.	Return to Main Menu\n";

	const std::string M_STR_ADDSCANNERDEVICES1 = "		Select an index of the device to register it\n";
	const std::string M_STR_ADDSCANNERDEVICES2 = "		0.	Return to Main Menu\n";


	// Text formatting strings 
	const std::string M_NEWLINE = "\n";
	const std::string M_TABLINE = "\t";

	void PrintScannerDeviceList();
	void ClearMenu();					// Clears all CLI text

public:
	void PrintMainMenu();				// Prints the main splash menu
	void PrintScannerDevicesMenu();		// Prints initial scanner devices menu
	void PrintAddScannerDevicesMenu();	// Prints a list of devices for selection when adding
};