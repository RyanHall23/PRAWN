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
	const std::string M_STR_EDITSCANNERDEVICES3 = "		3.	Overwrite Scanner\n";
	const std::string M_STR_EDITSCANNERDEVICES4 = "		0.	Return to Main Menu\n";

	// Add, Remove, Overwrite Menu Strings
	const std::string M_STR_ADDSCANNERDEVICES1			= "		Select an index of the device to REGISTER it\n";
	const std::string M_STR_REMOVESCANNERDEVICES1		= "		Select an index of the device to DEREGISTER it\n";
	const std::string M_STR_OVERWRITESCANNERDEVICES1	= "		Select an index of the device to OVERWRITE it\n";

	const std::string M_STR_ADDSCANNERDEVICES2 = "		0.	Return to Main Menu\n";


	// Text formatting strings 
	const std::string M_NEWLINE = "\n";
	const std::string M_TABLINE = "\t";

	void PrintAllConnectedDeviceList();				// Prints all connected devices
	void PrintRegisteredDeviceList();
	void ClearMenu();								// Clears all CLI text

public:
	void PrintMainMenu();				// Prints the main splash menu

	// Scanners Menu Nav Group
	void PrintScannersMainMenu();				// Prints initial scanner devices menu
	void PrintAddScannersDeviceMenu();			// Prints add scanner devices menu
	void PrintRemoveScannersDeviceMenu();		// Prints remove scanner devices menu
	void PrintOverwriteScannersDeviceMenu();	// Prints overwrite scanner devices menu
};