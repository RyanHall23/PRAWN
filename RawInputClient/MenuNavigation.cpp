#include "MenuNavigation.h"

std::string	CMenuNavigation::m_strNavigationDevice = "";	// Initialise static for storing device name that is navigating menu

CMenuNavigation::CMenuNavigation()
{

}

CMenuNavigation::~CMenuNavigation()
{

}

/// <summary>
/// Registers the menu navigation device autmoatically as a navigation device if not a registered device for speed checking
/// </summary>
void CMenuNavigation::RegisterNavigationDevice(std::string strDeviceName)
{
	m_strNavigationDevice = strDeviceName;
}

/// <summary>
/// Returns registered navigation device
/// </summary>
std::string CMenuNavigation::GetNavigationDevice()
{
	return m_strNavigationDevice;
}

/// <summary>
/// Open file window to allow user to select the database file they wish to use, and get directory
/// </summary>
/// <param name="filter"></param>
/// <param name="owner"></param>
/// <returns></returns>
std::string CMenuNavigation::OpenFile(char* filter, HWND owner)
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;	// OFN_NOCHANGEDIR - Restores back to default working directory, changing the save destination of the settings file
	ofn.lpstrDefExt = "";

	std::string strFileName;

	if (GetOpenFileName(&ofn))
		strFileName = fileName;

	return strFileName;
}


/// <summary>
/// Build user input command string member
/// </summary>
/// <param name="translatedKey"></param>
void CMenuNavigation::BuildCommand(char translatedKey)
{
	if (IsValidCharacter(translatedKey))
	{
		switch (translatedKey)
		{
		case m_cCarriageReturn:					// Is a carraige return key			(\r)
			if (m_strCmdMsg.length() > 0)		// If not empty (Something can be returned)
			{
				ProcessCommand(ReturnInputEvent());
				return;
			}
			break;
		case m_cEscape:							// Is a backspace key				(\b)
			if (m_strCmdMsg.length() > 0)		// If not empty (Something can be removed)
			{
				BackspaceInputEvent();
			}
			break;
		default:								// Is alphanumeric					(A-9)
			m_strCmdMsg += translatedKey;		// Add to string
			KeyInputEvent();
			break;
		}
	}
}

/// <summary>
/// Process user input command and navigate the CLI menus
/// </summary>
void CMenuNavigation::ProcessCommand(std::string cmdMsg)
{
	int iMenuInput = ConvertMenuInputToInt(cmdMsg);			// Convert to int to handle menu keys to correspond with options

	if (eMenuPosition == MainMenu)
	{
		if (iMenuInput == 1)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintScannersMainMenu();				// Print Scanner devices Main Menu
			eMenuPosition = ScannersMain;					// Set current menu position to ScannerDevices Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit Speed & Location screen entry
		{
			m_pMenuCli.PrintSpeedLocMainMenu();				// Print SpeedLoc Main menu
			eMenuPosition = SpeedLocationMenu;				// Set current menu position to Speed&Location Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 3)	// Edit Database screen entry
		{
			m_pMenuCli.PrintDBDirNameMenu();				// Print 
			eMenuPosition = EditDBDirectoryMenu;			// Set current menu position to DB Directory Edit Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
		if (iMenuInput == 4)	// Test database connection screen entry
		{
			m_pMenuCli.PrintDBTestMenuResult();				// Print DB Connection result
			eMenuPosition = TestDBConnectionMenu;			// Set current menu position to DB Connection Test Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

#pragma region Scanner Navigation
	if (eMenuPosition == ScannersMain)
	{
		if (iMenuInput == 0)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintMainMenu();						// Print Main Menu
			eMenuPosition = MainMenu;						// Set current menu position to MainMenu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 1)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintAddScannersDeviceMenu();		// Print Add Scanner devices menu
			eMenuPosition = ScannersAdd;					// Set current menu position to ScannersAdd
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintRemoveScannersDeviceMenu();		// Print Remove Scanner devices menu
			eMenuPosition = ScannersRemove;					// Set current menu position to ScannersRemove
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 3)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintOverwriteScannersDeviceMenu();	// Print Overwrite Scanner devices menu
			eMenuPosition = ScannersOverwrite;				// Set current menu position to ScannersOverwrite
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (eMenuPosition == ScannersAdd)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();				// Print Scanner devices menu
			eMenuPosition = ScannersMain;					// Set current menu position to ScannerMain Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0 && m_pDevProp.m_vecstrAllDevices.size() > 0 
			&& iMenuInput <= m_pDevProp.m_vecstrAllDevices.size())
		{
			if (m_pDevProp.RegisterDevice(iMenuInput - m_iDeviceListOffset, GetNavigationDevice()))
			{
				m_pMenuCli.PrintAddScannersDeviceMenu();		// Print Add Scanner devices menu
				m_pPersistence.SaveSettings(m_pDevProp);		// Save setting into .txt File
			}
			else
			{
				m_pMenuCli.PrintCannotAddDeviceError();	// Print error
			}
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (eMenuPosition == ScannersRemove)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();				// Print Scanner devices menu
			eMenuPosition = ScannersMain;					// Set current menu position to ScannerMain Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0 && m_pDevProp.m_vecstrRegisteredDevices.size() > 0 && iMenuInput <= m_pDevProp.m_vecstrRegisteredDevices.size())
		{
			m_pDevProp.DeregisterDevice(iMenuInput - m_iDeviceListOffset);	// DEREGISTER device, removing offset for print output
			m_pMenuCli.PrintRemoveScannersDeviceMenu();						// Print Remove Scanner devices menu for refresh
			m_pPersistence.SaveSettings(m_pDevProp);						// Save setting into .txt File
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (eMenuPosition == ScannersOverwrite)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();					// Print Scanner devices menu
			eMenuPosition = ScannersMain;						// Set current menu position to ScannerMain Menu
			m_bOverwriteDeregistered = FALSE;					// Reset Deregistered state to FALSE before overwriting
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0 && m_pDevProp.m_vecstrRegisteredDevices.size() > 0 && iMenuInput <= m_pDevProp.m_vecstrRegisteredDevices.size() && !m_bOverwriteDeregistered)
		{
			m_indexOldDevice = iMenuInput - m_iDeviceListOffset;	//	Store index of device to overwrite (minus the offset) 
			m_pMenuCli.PrintAddScannersDeviceMenu();			// Repurpose the Add Scanners device menu once device to be overwritten has been chosen
			m_bOverwriteDeregistered = TRUE;					// Set Deregistered state to TRUE, ready to be overwritten
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
		else if (iMenuInput > 0 && m_pDevProp.m_vecstrAllDevices.size() > 0 && iMenuInput <= m_pDevProp.m_vecstrAllDevices.size() && m_bOverwriteDeregistered)
		{
			m_indexNewDevice = iMenuInput - m_iDeviceListOffset;	// Store index of device to write (minus the offset) 

			if (m_pDevProp.OverwriteDevice(m_indexOldDevice, m_indexNewDevice, GetNavigationDevice()))
			{
				m_pMenuCli.PrintOverwriteScannersDeviceMenu();	// Reprint overwrite menu with new settings
				m_pPersistence.SaveSettings(m_pDevProp);		// Save setting into .txt File

				m_bOverwriteDeregistered = FALSE;				// Reset Deregistered state to FALSE before overwriting
				m_indexNewDevice = 0;								// Reset old new index
				m_indexOldDevice = 0;								// Reset old device index
				return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
			}
			else
			{
				m_pMenuCli.PrintCannotAddDeviceError();			// Print error
				return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
			}
		}
		return;								// Break out of ProcessCommand method to avoid multiple cases being triggered at once
	}
#pragma endregion

#pragma region Speed & Location Navigation
	if (eMenuPosition == SpeedLocationMenu)
	{
		if (iMenuInput == 0)	// Return to Main Menu
		{
			m_pMenuCli.PrintMainMenu();						// Print Main Menu
			eMenuPosition = MainMenu;						// Set current menu position to MainMenu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 1)	// Edit Speed Menu
		{
			m_pMenuCli.PrintEditSpeedMenu();				// Print Edit Speed Menu
			eMenuPosition = EditSpeed;						// Set current menu position to EditSpeed Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit Location Menu
		{
			m_pMenuCli.PrintEditLocationMenu();				// Print Edit Location Menu
			eMenuPosition = EditActualLocation;				// Set current menu position to EditLocation Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (eMenuPosition == EditSpeed)
	{
		if (iMenuInput == 0)	// Return to Speed&Location Main Menu
		{
			m_pMenuCli.PrintSpeedLocMainMenu();				// Print SpeedLocation Main Menu
			eMenuPosition = SpeedLocationMenu;				// Set current menu position to SpeedLocation Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0 )
		{
			if (m_pDevProp.OverwriteSpeedLimit(iMenuInput))	// Check if speed limit is a sensible legal limit
			{
				m_pMenuCli.PrintEditSpeedMenu();			// Reprint Edit Speed Menu with new setting
				m_pPersistence.SaveSettings(m_pDevProp);	// Save setting into .txt File
				return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
			}
			else
			{
				m_pMenuCli.PrintCannotEditSpeedError();		// Return with error that speed must be between 1 and 120
				return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
			}
			return;
		}
	}

	if (eMenuPosition == EditActualLocation)
	{
		if (iMenuInput == 0)	// Return to Speed&Location Menu
		{
			m_pMenuCli.PrintSpeedLocMainMenu();				// Print SpeedLocation Main Menu
			eMenuPosition = SpeedLocationMenu;				// Set current menu position to SpeedLocation Menu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
		else
		{
			m_pDevProp.OverwriteLocation(cmdMsg);			// Use string input type to overwrite actual location
			m_pMenuCli.PrintEditLocationMenu();				// Reprint menu to update text with new setting
			m_pPersistence.SaveSettings(m_pDevProp);		// Save setting into .txt File
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}


#pragma endregion

#pragma region Database Navigation
	if (eMenuPosition == EditDBDirectoryMenu)
	{
		if (iMenuInput == 0)	// Return to Main Menu
		{
			m_pMenuCli.PrintMainMenu();						// Print Main Menu
			eMenuPosition = MainMenu;						// Set current menu position to MainMenu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
		if (iMenuInput == 1)	// Open File Selector
		{
			std::string strNewDBFileLocation = OpenFile();
			m_pDevProp.OverwriteDatabaseDirectory(strNewDBFileLocation);	// Use cmdMsg to overwrite database directory with string
			m_pMenuCli.PrintDBDirNameMenu();				// Reperint database directory with new directory setting
			m_pPersistence.SaveSettings(m_pDevProp);		// Save setting into .txt File
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}
	if (eMenuPosition == TestDBConnectionMenu)
	{
		if (iMenuInput == 0)	// Return to Main Menu
		{
			m_pMenuCli.PrintMainMenu();						// Print Main Menu
			eMenuPosition = MainMenu;						// Set current menu position to MainMenu
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
		if (iMenuInput == 1)	// Database test
		{
			m_pDatabaseHelper.TestDatabase();
			m_pMenuCli.PrintDBTestMenuResult();
			return;							// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}
#pragma endregion
}

/// <summary>
/// Check if key is a valid AlphaNumeric Key, if true (Including \b && \r) return true, else special keys (,.;'[]~: etc) return false
/// </summary>
/// <param name="translatedKey"></param>
/// <returns></returns>
bool CMenuNavigation::IsValidCharacter(char translatedKey)
{
	return (translatedKey >= 0 && translatedKey <= 255 && translatedKey != KEY_SHIFT);
}

/// <summary>
/// Converts menu option inputs from string to integerm for comparison statements
/// </summary>
/// <param name="cmdMsg"></param>
/// <returns></returns>
int CMenuNavigation::ConvertMenuInputToInt(std::string cmdMsg)
{
	int validInput;

	try 
	{
		validInput = std::stoi(cmdMsg);
		if (validInput < 0)
		{
			validInput = -1;
		}
	}
	catch (const std::exception e) 
	{
		validInput = -1;
	}

	return validInput;		// Return
}

/// <summary>
/// Print text on standard key in event
/// </summary>
void CMenuNavigation::KeyInputEvent()
{
	std::cout << m_cCarriageReturn << m_strCmdMsg << std::flush;
}

/// <summary>
/// Clear text after return key event
/// </summary>
std::string CMenuNavigation::ReturnInputEvent()
{
	ClearInputLine();
	std::string strReturnInput = m_strCmdMsg;
	m_strCmdMsg.resize(0);

	return strReturnInput;
}

/// <summary>
/// Print text on backspace event
/// </summary>
void CMenuNavigation::BackspaceInputEvent()
{
	ClearInputLine();
	m_strCmdMsg.resize(m_strCmdMsg.length() - 1);
	std::cout << m_cCarriageReturn << m_strCmdMsg << std::flush;
}

/// <summary>
/// Clear line with \t to overwrite any input
/// </summary>
void CMenuNavigation::ClearInputLine()
{
	std::cout << m_cCarriageReturn << OVERWRITEBLANK << m_cCarriageReturn << std::flush;
}
