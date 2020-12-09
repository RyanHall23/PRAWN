#include "MenuNavigation.h"

CMenuNavigation::CMenuNavigation()
{

}

CMenuNavigation::~CMenuNavigation()
{
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
	int iMenuInput = ConvertMenuInputToInt(cmdMsg);	// Convert to int to handle menu key presses directly with enum literals


	if (mmCurrentMenu == MainMenu)
	{
		if (iMenuInput == 1)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintScannersMainMenu();		// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;				// Set current menu position to ScannerDevicesMenu
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit Speed & Location screen entry
		{
			// TODO Add speed/location screen and change enum

			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 3)	// Edit Database screen entry
		{
			// TODO Add Database screen and change enum
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

#pragma region Scanner Navigation
	if (mmCurrentMenu == ScannersMain)
	{
		if (iMenuInput == 0)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintMainMenu();					// Print Main Menu
			mmCurrentMenu = MainMenu;					// Set current menu position to MainMenu
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 1)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintAddScannersDeviceMenu();	// Print Add Scanner devices menu
			mmCurrentMenu = ScannersAdd;				// Set current menu position to ScannersAdd
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 2)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintRemoveScannersDeviceMenu();	// Print Remove Scanner devices menu
			mmCurrentMenu = ScannersRemove;				// Set current menu position to ScannersRemove
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput == 3)	// Edit Scanners screen entry
		{
			m_pMenuCli.PrintOverwriteScannersDeviceMenu();	// Print Overwrite Scanner devices menu
			mmCurrentMenu = ScannersOverwrite;			// Set current menu position to ScannersOverwrite
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}
	}

	if (mmCurrentMenu == ScannersAdd)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();		// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;				// Set current menu position to ScannerMain Menu
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0)
		{
			// TODO add logic to scanners on selected index
		}
	}

	if (mmCurrentMenu == ScannersRemove)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();		// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;				// Set current menu position to ScannerMain Menu
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0)
		{
			// TODO remove logic to scanners on selected index
		}
	}

	if (mmCurrentMenu == ScannersOverwrite)
	{
		if (iMenuInput == 0)	// Return to Scanners Main
		{
			m_pMenuCli.PrintScannersMainMenu();		// Print Scanner devices menu
			mmCurrentMenu = ScannersMain;				// Set current menu position to ScannerMain Menu
			return;										// Break out of ProcessCommand method to avoid multiple cases being triggered at once
		}

		if (iMenuInput > 0)
		{
			// TODO overwrite logic to scanners on selected index (combine remove->add?)
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
