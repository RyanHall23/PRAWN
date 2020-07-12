#include "MenuCLI.h"

/// <summary>
/// Prints the main menu splash screen in the console
/// </summary>
void CMenuCLI::PrintMainMenu()
{
	std::cout << M_STR_SOFTWARETITLE << M_NEWLINE<< M_STR_MAINMENULINE1 << M_STR_MAINMENULINE2	<< M_NEWLINE << M_STR_COPYRIGHTNOTICE;
}
