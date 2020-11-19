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

	enum MainMenuPosition
	{
		MainMenu = 0,
		ScannerDevicesMenu,
		SpeedAndLocationMenu,
		DatabaseLocationMenu
	};

	enum ScannerDevicesPosition
	{
		returnFromDevices = 0,
		AddScannerDevicesMenu,
		ChangeScannerDevicesMenu,
		RemoveScannerDevicesMenu
	};

	enum SpeedAndLocationMenuPosition
	{
		SpeedAndLocationMainMenu = 0
	};

	enum DatabaseLocationMenuPosition
	{
		DatabaseLocatioMainMenu = 0
	};

	MainMenuPosition mainMenuPosition = MainMenu;

public:
	CMenuNavigation();
	~CMenuNavigation();

	// Menu navigation tools
	void BuildCommand(char translatedKey);
	void ProcessCommand(std::string cmdMsg);
	bool IsAlphaNumeric(char translatedKey);
	int  ConvertMenuInputToInt(std::string cmdMsg);

	void KeyInputEvent();
	std::string ReturnInputEvent();
	void BackspaceInputEvent();
	void ClearInputLine();
};