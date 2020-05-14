#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <windows.h>

class CDeviceProperties
{
public:
	CDeviceProperties();
	~CDeviceProperties();

	void ReadDeviceProperties();

	std::string		m_strScannerAName;			// Scanner A device name
	std::string		m_strScannerBName;			// Scanner B device name
	std::string		m_strScannerLocation;		// Scanner Location
	int				m_iSpeedLimit;				// Speed limit at location
	float			m_fScannerDistance;			// Distance between scanners

};