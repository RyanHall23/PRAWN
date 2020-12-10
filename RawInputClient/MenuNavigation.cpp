#include "MenuNavigation.h"
std::string		CMenuNavigation::m_strNavigationDevice = "";	// Initialise static for storing device name that is navigating menu

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
/// Build user input command string member
/// </summary>
/// <param name="translatedKey"></param>
void CMenuNavigation::BuildCommand(char translatedKey)
{
	if (IsAlphaNumeric(translatedKey))
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


	if (mmCurrentMenu == MainMenu)
	{
		if (iMenuInput == 1)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintScannersMainMenu();				// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;					// Set current menu position to ScannerDevicesMenu
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit Speed & Location screen entry
		{
			m_pMenuCli.PrintSpeedLocMainMenu();
			mmCurrentMenu = SpeedLocationMenu;
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 3)	// Edit Database screen entry
		{
			m_pMenuCli.PrintDBDirNameMenu();
			mmCurrentMenu = EditDBDirectoryMenu;
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

#pragma region Scanner Navigation
	if (mmCurrentMenu == ScannersMain)
	{
		if (iMenuInput == 0)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintMainMenu();						// Print Main Menu
			mmCurrentMenu = MainMenu;						// Set current menu position to MainMenu
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 1)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintAddScannersDeviceMenu();		// Print Add Scanner devices menu
			mmCurrentMenu = ScannersAdd;					// Set current menu position to ScannersAdd
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintRemoveScannersDeviceMenu();		// Print Remove Scanner devices menu
			mmCurrentMenu = ScannersRemove;					// Set current menu position to ScannersRemove
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 3)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintOverwriteScannersDeviceMenu();	// Print Overwrite Scanner devices menu
			mmCurrentMenu = ScannersOverwrite;				// Set current menu position to ScannersOverwrite
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (mmCurrentMenu == ScannersAdd)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();				// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;					// Set current menu position to ScannerMain Menu
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0 && m_pDevProp.m_vecstrAllDevices.size() > 0 && iMenuInput <= m_pDevProp.m_vecstrAllDevices.size())
		{
			if (m_pDevProp.RegisterDevice(iMenuInput - m_iDeviceListOffset, GetNavigationDevice()))
			{
				m_pMenuCli.PrintAddScannersDeviceMenu();						// Print Add Scanner devices menu
				m_pPersistence.SaveSettings(m_pDevProp);
			}
			else
			{
				m_pMenuCli.PrintCannotAddDeviceError();	// Print error
			}
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (mmCurrentMenu == ScannersRemove)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();				// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;					// Set current menu position to ScannerMain Menu
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0 && m_pDevProp.m_vecstrRegisteredDevices.size() > 0 && iMenuInput <= m_pDevProp.m_vecstrRegisteredDevices.size())
		{
			m_pDevProp.DeregisterDevice(iMenuInput - m_iDeviceListOffset);	// DEREGISTER device, removing offset for print output
			m_pMenuCli.PrintRemoveScannersDeviceMenu();						// Print Remove Scanner devices menu for refresh
			m_pPersistence.SaveSettings(m_pDevProp);
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (mmCurrentMenu == ScannersOverwrite)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();				// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;					// Set current menu position to ScannerMain Menu
			return;					// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0)
		{
			return;					// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}
#pragma endregion

#pragma region Speed & Location Navigation
	if (mmCurrentMenu == SpeedLocationMenu)
	{
		if (iMenuInput == 0)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintMainMenu();						// Print Main Menu
			mmCurrentMenu = MainMenu;						// Set current menu position to MainMenu
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 1)	// Edit  screen entry
		{
			m_pMenuCli.PrintEditSpeedMenu();				// Print Add  devices menu
			mmCurrentMenu = EditSpeed;						// Set current menu position to 
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit  screen entry
		{
			m_pMenuCli.PrintEditLocationMenu();				// Print Remove  devices menu
			mmCurrentMenu = EditActualLocation;				// Set current menu position to 
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (mmCurrentMenu == EditSpeed)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintSpeedLocMainMenu();
			mmCurrentMenu = SpeedLocationMenu;
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0 )
		{
			if (m_pDevProp.OverwriteSpeedLimit(iMenuInput))
			{
				m_pMenuCli.PrintEditSpeedMenu();
				m_pPersistence.SaveSettings(m_pDevProp);
				return;
			}
			else
			{
				m_pMenuCli.PrintCannotEditSpeedError();
				return;
			}
			return;
		}
	}

	if (mmCurrentMenu == EditActualLocation)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintSpeedLocMainMenu();
			mmCurrentMenu = SpeedLocationMenu;
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
		else
		{
			m_pDevProp.OverwriteLocation(cmdMsg);
			m_pPersistence.SaveSettings(m_pDevProp);
			m_pMenuCli.PrintEditLocationMenu();
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}


#pragma endregion

#pragma region Database Navigation
	if (EditDBDirectoryMenu)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintMainMenu();						// Print Main Menu
			mmCurrentMenu = MainMenu;						// Set current menu position to MainMenu
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
		else
		{
			m_pDevProp.OverwriteDatabaseDirectory(cmdMsg);
			m_pPersistence.SaveSettings(m_pDevProp);
			m_pMenuCli.PrintDBDirNameMenu();
			return;						// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}
#pragma endregion


}

/// <summary>
/// Check if key is a valid AlphaNumeric Key, if true (Including \b && \r) return true, else special keys (,.;'[]~: etc) return false
/// </summary>
/// <param name="translatedKey"></param>
/// <returns></returns>
bool CMenuNavigation::IsAlphaNumeric(char translatedKey)
{
	if (translatedKey >= 0 && translatedKey <= 255)
	{
		return true;
	}
	return false;
}

/// <summary>
/// Converts menu option inputs from string to integerm for comparison statements
/// </summary>
/// <param name="cmdMsg"></param>
/// <returns></returns>
int CMenuNavigation::ConvertMenuInputToInt(std::string cmdMsg)
{
	try 
	{
		int validInput;
		validInput = std::stoi(cmdMsg);
		if (validInput >= 0)
		{
			return validInput;
		}
	}
	catch (const std::exception e) 
	{
		return -1;	// Return
	}

	return -1;		// Return
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
