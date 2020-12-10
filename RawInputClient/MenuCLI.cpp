#include "MenuCLI.h"

/// <summary>
/// Print the main menu splash screen in the console
/// </summary>
void CMenuCLI::PrintMainMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE<< M_STR_MAINMENULINE1 << M_STR_MAINMENULINE2 << M_STR_MAINMENULINE3 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
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
/// Print the all connected devices scanner device list
/// </summary>
void CMenuCLI::PrintAllConnectedDeviceList()
{
	// Don't clear this menu as it is printed under a titled screen
	std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());
	
	for (int i = 0; i < pPersistence->m_vecstrAllDevices.size(); ++i)
	{
		std::cout << M_TABLINE << M_TABLINE << i+1 << ".	" << pPersistence->m_vecstrAllDevices.at(i) << M_NEWLINE;
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