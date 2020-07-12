#include "MenuNavigation.h"

CMenuNavigation::CMenuNavigation()
{
}

CMenuNavigation::~CMenuNavigation()
{
}

std::string CMenuNavigation::BuildCommand(char translatedKey)
{
	if (isdigit(translatedKey) || isalpha(translatedKey))                 // Check for valid alphanumeric key
	{
		m_vecstrCmdMsg.push_back(translatedKey);	// Add character to vector to build registration

		m_strTempCmd = "";
		for (unsigned int i = 0; i < m_vecstrCmdMsg.size(); i++)	// Increment through size of licence plate
		{
			m_strTempCmd += m_vecstrCmdMsg.at(i);	// Build as a string
		}
		PrintMenuInput();

	}
	else if (translatedKey == m_cReturnKeyVirtualCode)          // Check if Return Key/End of string key has been entered
	{
		if (m_vecstrCmdMsg.capacity() > 0) // If licence plate is populated before return is hit
		{
			for (unsigned int i = 0; i < m_vecstrCmdMsg.size(); i++)	// Increment through size of licence plate
			{
				m_ssCmdMsgBuilder << m_vecstrCmdMsg.at(i);	// Build as a string
			}

			m_strTempCmd = m_ssCmdMsgBuilder.str();		// Convert to temp string
			m_strCompletedCmdMsg = m_strTempCmd;		// Assign completed registration

			m_strTempCmd = "";					// Clear temp string
			m_ssCmdMsgBuilder.str("");				// Clear strigstream
			m_vecstrCmdMsg.clear();				// Clear vector

#ifdef _DEBUG
			OutputDebugString("\n Menu Input: ");
			OutputDebugString((LPCSTR)m_strCompletedCmdMsg.c_str());   // Debug output device name
			OutputDebugString("\n");
#endif
			return m_strCompletedCmdMsg;	// Return licence plate registration
		}
	}
	else if (translatedKey == '\b')
	{
		m_strTempCmd = "";
		if (m_vecstrCmdMsg.size() > 0)
		{

			m_vecstrCmdMsg.erase(m_vecstrCmdMsg.begin() + m_vecstrCmdMsg.size());
			for (unsigned int i = 0; i < m_vecstrCmdMsg.size(); i++)	// Increment through size of licence plate
			{
				m_strTempCmd += m_vecstrCmdMsg.at(i);	// Build as a string
			}
		}
		
		PrintMenuInput();

	}
	return "";
}

void CMenuNavigation::PrintMenuInput()
{
	std::cout << "\r" << m_strTempCmd << std::flush;
}
