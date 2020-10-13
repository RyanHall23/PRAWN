#include "MenuCLI.h"

/// <summary>
/// Prints the main menu splash screen in the console
/// </summary>
void CMenuCLI::PrintMainMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE<< M_STR_MAINMENULINE1 << M_STR_MAINMENULINE2	<< M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

void CMenuCLI::PrintScannerDevicesMenu()
{
	ClearMenu();
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE << M_STR_EDITSCANNERDEVICES1 << M_STR_EDITSCANNERDEVICES2 << M_STR_EDITSCANNERDEVICES3 << M_NEWLINE << M_STR_EDITSCANNERDEVICES4 << M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}

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