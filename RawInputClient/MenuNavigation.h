#pragma once
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



public:
	CMenuNavigation();
	~CMenuNavigation();

	// Menu navigation tools
	std::string BuildCommand(char translatedKey);
	bool IsAlphaNumeric(char translatedKey);


	void KeyInputEvent();
	std::string ReturnInputEvent();
	void BackspaceInputEvent();
	void ClearInputLine();
};