#pragma once

#include "MenuCLI.h"

#include <string>
#include <iostream>

#include <windows.h>

class CMenuNavigation
{
private:
	std::string					m_strCmdMsg;

	const std::string			NULLSTRING = "";
	const std::string			OVERWRITEBLANK = "\t\t\t\t\t\t\t\t\t";

	static constexpr char		m_cNewLine = '\n';
	static constexpr char		m_cEscape = '\b';
	static constexpr char		m_cCarriageReturn = '\r';

	CMenuCLI m_pMenuCli;

	enum MenuPosition
	{
		MainMenu = 1,
		ScannerDevicesMenu,
	};

	MenuPosition currentMenuPosition = MainMenu;

public:
	CMenuNavigation();
	~CMenuNavigation();

	// Menu navigation tools
	void BuildCommand(char translatedKey);
	void ProcessCommand(std::string cmdMsg);
	bool IsAlphaNumeric(char translatedKey);


	void KeyInputEvent();
	std::string ReturnInputEvent();
	void BackspaceInputEvent();
	void ClearInputLine();
};