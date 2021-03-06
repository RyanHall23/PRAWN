#pragma once

#include <string>
#include <iostream>
#include <windows.h>

#include "Persistence.h"
#include "DatabaseHelper.h"

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
	const std::string M_STR_MAINMENULINE4		= "		4.	Test Database Connection\n";
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
	const std::string M_STR_CANNOTADDDEVICEERROR1 = "		Cannot REGSITER the NAVIGATION device or a device that has already been REGISTERED\n";

	// Edit Speed & Location Menu String
	const std::string M_STR_EDITSPEEDLOC1 = "		1.	Edit Speed\n";
	const std::string M_STR_EDITSPEEDLOC2 = "		2.	Edit Location\n";
	const std::string M_STR_EDITSPEEDLOC3 = "		0.	Return to Main Menu\n";

	// Edit Speed, Location strings
	const std::string M_STR_EDITSPEED1	= "		Enter the desired SPEED in MPH \n";
	const std::string M_STR_EDITSPEED2	= "		Speed is currently set to:	";

	const std::string M_STR_EDITLOC1	= "		Enter the desired LOCATION \n";
	const std::string M_STR_EDITLOC2	= "		Location is currently set to:	";

	const std::string M_STR_CANNOTEDITSPEEDERROR1 = "		Speed is too HIGH, it MUST be UNDER 120 MPH and ABOVE 0 MPH\n";

	// Edit Database Location/Filename Strings
	const std::string M_STR_EDITDBDIR1 = "		Navigate to the desried database and select the file \n";
	const std::string M_STR_EDITDBDIR2 = "		Database Directory is currently set to:	";
	const std::string M_STR_EDITDBDIR3 = "		1.	Select Database File\n";
	const std::string M_STR_EDITDBDIR4 = "		0.	Return to Main Menu\n";

	// Database Test Result Strings
	const std::string M_STR_TESTDB1 = "			Test the connection to the database: ";
	const std::string M_STR_TESTDB2 = "			Database connection result: ";
	const std::string M_STR_TESTDB3 = "		1.	Test Database \n";
	const std::string M_STR_TESTDB4 = "		0.	Return to Main Menu\n";

	static std::string m_strDBTestResult; // = "A Test has not been ran";

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

	void PrintCannotAddDeviceError();

	// Speed Location Menu Nav Group

	void PrintSpeedLocMainMenu();
	void PrintEditSpeedMenu();
	void PrintEditLocationMenu();

	void PrintCannotEditSpeedError();

	// Database Location/Name Menu Nav Group

	void PrintDBDirNameMenu();
	void PrintDBTestMenuResult();
	void UpdateDatabaseResultString(std::string strResult);
};