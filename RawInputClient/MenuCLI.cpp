#include "MenuCLI.h"

std::string	CMenuCLI::m_strDBTestResult = "A Test has not been ran";	// Initialise static for storing database test results


/// <summary>
/// Print the main menu splash screen in the console
/// </summary>
void CMenuCLI::PrintMainMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE<< M_STR_MAINMENULINE1 << M_STR_MAINMENULINE2 << M_STR_MAINMENULINE3 << M_STR_MAINMENULINE4 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Print the Scanner main menu in the console
/// </summary>
void CMenuCLI::PrintScannersMainMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_EDITSCANNERDEVICES1 << M_STR_EDITSCANNERDEVICES2 << M_STR_EDITSCANNERDEVICES3 << M_NEWLINE << M_STR_EDITSCANNERDEVICES4 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Print add scanner menu to the console
/// </summary>
void CMenuCLI::PrintAddScannersDeviceMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_ADDSCANNERDEVICES1 << M_NEWLINE;
	PrintAllConnectedDeviceList();
	std::cout << M_NEWLINE << M_STR_ADDSCANNERDEVICES2 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Print remove scanner menu to the console
/// </summary>
void CMenuCLI::PrintRemoveScannersDeviceMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_REMOVESCANNERDEVICES1 << M_NEWLINE;
	PrintRegisteredDeviceList();
	std::cout << M_NEWLINE << M_STR_ADDSCANNERDEVICES2 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Print Overwrite scanner menu to the console
/// </summary>
void CMenuCLI::PrintOverwriteScannersDeviceMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_OVERWRITESCANNERDEVICES1 << M_NEWLINE;
	PrintRegisteredDeviceList();
	std::cout << M_NEWLINE << M_STR_ADDSCANNERDEVICES2 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Reprint the scanners device menu with a suitable warning for not being able to add the selected device
/// </summary>
void CMenuCLI::PrintCannotAddDeviceError()
{
	PrintAddScannersDeviceMenu();
	std::cout << M_NEWLINE << M_STR_CANNOTADDDEVICEERROR1 << M_NEWLINE;
}

/// <summary>
/// Print main speed & location menu
/// </summary>
void CMenuCLI::PrintSpeedLocMainMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_EDITSPEEDLOC1 << M_STR_EDITSPEEDLOC2 << M_NEWLINE << M_STR_EDITSPEEDLOC3 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Print edit speed menu screen
/// </summary>
void CMenuCLI::PrintEditSpeedMenu()
{
	std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_EDITSPEED1 << M_NEWLINE << M_STR_EDITSPEED2 << pPersistence->m_iSpeedLimit << M_NEWLINE << M_NEWLINE << M_STR_EDITSPEEDLOC3 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Print edit phyiscal location menu screen
/// </summary>
void CMenuCLI::PrintEditLocationMenu()
{
	std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_EDITLOC1 << M_NEWLINE << M_STR_EDITLOC2 << pPersistence->m_strScannerLocation << M_NEWLINE << M_NEWLINE << M_STR_EDITSPEEDLOC3 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Prints an error if the new speed edit is too high of a legal speed limit
/// </summary>
void CMenuCLI::PrintCannotEditSpeedError()
{
	PrintEditSpeedMenu();
	std::cout << M_NEWLINE << M_STR_CANNOTEDITSPEEDERROR1 << M_NEWLINE;
}

/// <summary>
/// Print edit DB name / location menu screen
/// </summary>
void CMenuCLI::PrintDBDirNameMenu()
{
	std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_EDITDBDIR1 << M_NEWLINE << M_STR_EDITDBDIR2 << pPersistence->m_strDatabaseDirectory << M_NEWLINE << M_NEWLINE << M_STR_EDITDBDIR3 << M_NEWLINE << M_STR_EDITDBDIR4 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

/// <summary>
/// Print Database test results menu
/// </summary>
void CMenuCLI::PrintDBTestMenuResult()
{
	std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_TESTDB1 << pPersistence->m_strDatabaseDirectory << M_NEWLINE << M_STR_TESTDB2 << m_strDBTestResult << M_NEWLINE << M_NEWLINE << M_STR_TESTDB3 << M_STR_TESTDB4;
}

/// <summary>
/// Print the all connected devices scanner device list
/// </summary>
void CMenuCLI::PrintAllConnectedDeviceList()
{
	// Don't clear this menu as it is printed under a titled screen
	std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

	std::vector<std::string> unregisteredDevices;	// Unregistered devices vector to display devices that are able to be added (with exception to the UI device)

	for (int i = 0; i < pPersistence->m_vecstrAllDevices.size(); ++i)
	{
		if (std::find(pPersistence->m_vecstrRegisteredDevices.begin(), pPersistence->m_vecstrRegisteredDevices.end(), pPersistence->m_vecstrAllDevices.at(i)) == pPersistence->m_vecstrRegisteredDevices.end())	// Check to see if it exists in the registered devices vector and whether to display it or not
		{
			unregisteredDevices.push_back(pPersistence->m_vecstrAllDevices.at(i));	// Add to display device vector
		}
	}

	for (int i = 0; i < unregisteredDevices.size(); ++i)
	{
		std::cout << M_TABLINE << M_TABLINE << i+1 << ".	" << unregisteredDevices.at(i) << M_NEWLINE;
	}
}

/// <summary>
/// Print all registered devices
/// </summary>
void CMenuCLI::PrintRegisteredDeviceList()
{
	// Don't clear this menu as it is printed under a titled screen
	std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

	for (int i = 0; i < pPersistence->m_vecstrRegisteredDevices.size(); ++i)
	{
		std::cout << M_TABLINE << M_TABLINE << i + 1 << ".	" << pPersistence->m_vecstrRegisteredDevices.at(i) << M_NEWLINE;
	}
}

/// <summary>
/// Updates Result string from database connection test
/// </summary>
/// <param name="strResult"></param>
void CMenuCLI::UpdateDatabaseResultString(std::string strResult)
{
	m_strDBTestResult = strResult;
}

/// <summary>
/// Clears all text from the CLI
/// </summary>
void CMenuCLI::ClearMenu()
{
	DWORD n;
	DWORD size;
	COORD coord = { 0 };
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h, &csbi);
	size = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(h, TEXT(' '), size, coord, &n);
	GetConsoleScreenBufferInfo(h, &csbi);
	FillConsoleOutputAttribute(h, csbi.wAttributes, size, coord, &n);
	SetConsoleCursorPosition(h, coord);
}