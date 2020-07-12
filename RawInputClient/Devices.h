#pragma once

#include <string>
#include <vector>

// Devices class
// Responsible for managing device properties E.G. HID Name
class CDevices
{
public:
	CDevices();		// Constructor
	~CDevices();	// Destructor

	std::string TruncateHIDName(std::string strHidName);		// Function that shortens HID name

	const unsigned short	m_sKeyDownFlag			= 1;		// For checking if the keyboard flag is "down"
};