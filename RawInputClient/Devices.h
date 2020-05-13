#pragma once

#include <atlstr.h>
#include <string>
#include <vector>

// Devices class
// Responsible for managing device properties E.G. HID Name
class CDevices
{
public:
	CDevices();		// Constructor
	~CDevices();		// Destructor

	CString TruncateHIDName(std::string strHidName);		// Function that shortens HID name


	const unsigned short	sKeyDownFlag			= 1;		// For checking if the keyboard flag is "down"
	const unsigned char		cReturnKeyVirtualCode	= 13;		// For checking if the used key is a Return key
	const CString			cstrReturnKeyMessage	= "!";		// Marks the end of the string with "return" notifier

private:
};