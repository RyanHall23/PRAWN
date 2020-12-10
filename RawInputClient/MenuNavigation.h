#pragma once

#include "MenuCLI.h"
#include "Persistence.h"

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

	const int					m_iDeviceListOffset = 1;
	static std::string			m_strNavigationDevice;	// For storing device name that is navigating menu

	BOOL m_bOverwriteDeregistered = FALSE;
	int indexOldDevice = 0;
	int indexNewDevice = 0;

	CPersistence	m_pPersistence;
	CMenuCLI		m_pMenuCli;
	CPersistence::DeviceProperties	m_pDevProp;

	enum MainMenuPosition
	{
		MainMenu			= 0,	

		// Scanners Menu Navigation group
		ScannersMain		= 10,	
		ScannersAdd			= 11,
		ScannersRemove		= 12,
		ScannersOverwrite	= 13,

		// Speed & Location Menu Navigation group
		SpeedLocationMenu	= 20, 	
		EditSpeed			= 21,
		EditActualLocation	= 22,

		// Database Menu Navigation group
		EditDBDirectoryMenu = 30
	};

	MainMenuPosition mmCurrentMenu = MainMenu;

public:
	CMenuNavigation();
	~CMenuNavigation();

	void RegisterNavigationDevice(std::string strDeviceName);
	std::string GetNavigationDevice();

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