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
	double			m_dbScannerDistance;		// Distance between scanners			(Metres)
	double			m_dbMaximumTravelTime;		// Expected Travel time between scanners
	std::string		m_strDatabaseDirectory;		// Database location on disc & name

	double			m_dbSecondsInHour = 3600;	// For speed/time calculations
	double			m_dbMPHToMiles = 1609.344;	// For speed/time calculations
};