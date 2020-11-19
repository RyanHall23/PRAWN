#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>

#include <windows.h>

#include "Devices.h"

class CPersistence
{
public:
	CPersistence();
	~CPersistence();

	struct DeviceProperties;

	// void SaveSettings(CDevices devicesConfig);
	std::unique_ptr<CPersistence::DeviceProperties> ReadSettings();

	std::string TruncateHIDName(std::string deviceName);

	const unsigned short	m_sKeyDownFlag			= 1;		// For checking if the keyboard flag is "down"
};

struct CPersistence::DeviceProperties
{
	static std::vector<std::string> m_vecstrAllDevices;			// Vector of all availible devices
	static std::vector<std::string> m_vecstrSelectedDevices;	// Vector of selected devices in loop
	static std::string				m_strScannerLocation;		// Scanner Location
	static int						m_iSpeedLimit;				// Speed limit at location				(MPH)
	static double					m_dbScannerDistance;		// Distance between scanners			(Metres)
	static double					m_dbOptimumTravelTime;		// Expected Travel time between scanners
	static std::string				m_strDatabaseDirectory;		// Database location on disc & name
	const double					m_dbSecondsInHour = 3600;	// For speed/time calculations
	const double					m_dbMPHToMiles = 1609.344;	// For speed/time calculations

	void CalculateMaximumTravelTime()
	{
		double dbSpeedLimitMetres = m_iSpeedLimit * m_dbMPHToMiles;
		m_dbOptimumTravelTime = (m_dbScannerDistance / (dbSpeedLimitMetres / m_dbSecondsInHour));
		m_dbOptimumTravelTime += (m_dbOptimumTravelTime / 10);
	}
};