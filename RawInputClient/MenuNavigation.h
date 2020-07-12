#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include <windows.h>

class CMenuNavigation
{
private:
	std::vector<char>	m_vecstrCmdMsg;				
	std::string					m_strTempCmd;				
	std::string					m_strCompletedCmdMsg;		
	std::stringstream			m_ssCmdMsgBuilder;			
	const char					m_cReturnKeyVirtualCode = 13;

public:
	CMenuNavigation();
	~CMenuNavigation();

	// Menu navigation tools
	std::string BuildCommand(char translatedKey);
	void PrintMenuInput();
};