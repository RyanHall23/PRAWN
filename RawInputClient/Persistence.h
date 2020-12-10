#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <windows.h>

class CPersistence
{
public:
	CPersistence();
	~CPersistence();

	struct DeviceProperties;

	void SaveSettings(DeviceProperties devicesConfig);
	std::unique_ptr<CPersistence::DeviceProperties> ReadSettings();

	std::string TruncateHIDName(std::string deviceName);

	const unsigned short	m_sKeyDownFlag			= 1;		// For checking if the keyboard flag is "down"
	const std::string		SETTINGSPACER			= "\n\n";	// Space settings apart for easy readability

};

struct CPersistence::DeviceProperties
{
	static std::vector<std::string> m_vecstrAllDevices;			// Vector of all availible devices
	static std::vector<std::string> m_vecstrRegisteredDevices;	// Vector of selected devices in loop
	static std::string				m_strScannerLocation;		// Scanner Location
	static int						m_iSpeedLimit;				// Speed limit at location				(MPH)
	static double					m_dbScannerDistance;		// Distance between scanners			(Metres)
	static double					m_dbOptimumTravelTime;		// Expected Travel time between scanners
	static std::string				m_strDatabaseDirectory;		// Database location on disc & name
	const double					m_dbSecondsInHour = 3600;	// For speed/time calculations
	const double					m_dbMPHToMiles = 1609.344;	// For speed/time calculations

	/// <summary>
	/// Calculate lowest possible travel time between each node
	/// </summary>
	void CalculateMaximumTravelTime()
	{
		double dbSpeedLimitMetres = m_iSpeedLimit * m_dbMPHToMiles;
		m_dbOptimumTravelTime = (m_dbScannerDistance / (dbSpeedLimitMetres / m_dbSecondsInHour));
		m_dbOptimumTravelTime += (m_dbOptimumTravelTime / 10);
	}

	/// <summary>
	/// Register device, by checking if it is registered or a 
	/// navigation device first before adding it to the registered devices vector
	/// </summary>
	/// <param name="index"></param>
	/// <param name="strNavigationDevice"></param>
	/// <returns></returns>
	BOOL RegisterDevice(int index, std::string strNavigationDevice)
	{
		// To check if it is a navigation device or already registered
		if (strNavigationDevice == m_vecstrAllDevices.at(index) || 
			std::find(m_vecstrRegisteredDevices.begin(), m_vecstrRegisteredDevices.end(), m_vecstrAllDevices.at(index)) != m_vecstrRegisteredDevices.end())
		{
			return FALSE;
		}

		m_vecstrRegisteredDevices.push_back(m_vecstrAllDevices.at(index));	// Add from all devices to registered devices via index

		return TRUE;
	}

	/// <summary>
	/// Deregister device by removing it from Registered Devices vector
	/// </summary>
	/// <param name="index"></param>
	void DeregisterDevice(int index)
	{
		m_vecstrRegisteredDevices.erase(m_vecstrRegisteredDevices.begin() + index);	// Remove from registered devices vector
	}
};