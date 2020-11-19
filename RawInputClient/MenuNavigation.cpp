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
		case m_cCarriageReturn:					// Is a carraige return key		(\r)
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
	int iMenuInput = ConvertMenuInputToInt(cmdMsg); // Convert to int to handle menu key presses directly with enum literals

	if (mainMenuPosition == MainMenu)
	{
		switch (iMenuInput)
		{
		case ScannerDevicesMenu:
				m_pMenuCli.PrintScannerDevicesMenu();		// Print Scanner devices menu
				mainMenuPosition = ScannerDevicesMenu;		// Set current menu position to ScannerDevicesMenu
			break;
		case SpeedAndLocationMenu:
			break;
		case DatabaseLocationMenu:
			break;
		}
	}
	else if (mainMenuPosition == ScannerDevicesMenu)
	{
		switch (iMenuInput)
		{
		case AddScannerDevicesMenu:
			m_pMenuCli.PrintAddScannerDevicesMenu();		// Print Scanner devices menu
			break;
		case ChangeScannerDevicesMenu:
			break;
		case RemoveScannerDevicesMenu:
			break;
		case returnFromDevices:
			m_pMenuCli.PrintMainMenu();		// Print Scanner devices menu
			mainMenuPosition = MainMenu;		// Set current menu position to ScannerDevicesMenu
		}
	}


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
/// Converts menu option inputs "2" to allow for easy switch case statements 
/// </summary>
/// <param name="cmdMsg"></param>
/// <returns></returns>
int CMenuNavigation::ConvertMenuInputToInt(std::string cmdMsg)
{
	try 
	{
		int validInput;
		validInput = std::stoi(cmdMsg);
		if (validInput > 0);
		{
			return validInput;
		}
	}
	catch (const std::exception& e) 
	{
		return -1;	// Return
	}
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
