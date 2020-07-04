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
	void CalculateMaximumTravelTime();

	std::string		m_strScannerAName;			// Scanner A device name
	std::string		m_strScannerBName;			// Scanner B device name
	std::string		m_strScannerLocation;		// Scanner Location
	int				m_iSpeedLimit;				// Speed limit at location				(MPH)
	float			m_fScannerDistance;			// Distance between scanners			(Metres)
	float			m_fMaximumTravelTime;		// Expected Travel time between scanners

	int				m_iSecondsInHour = 3600;
	float			m_fMPHToMiles = 1609.344;
};