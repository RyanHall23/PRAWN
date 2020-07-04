#include "DeviceProperties.h"
#include "Devices.h"

CDeviceProperties::CDeviceProperties()
{
	m_strScannerAName		= "";
	m_strScannerBName		= "";
	m_strScannerLocation	= "";
	m_iSpeedLimit			= 0;
	m_dbScannerDistance		= 0.00;
	m_dbMaximumTravelTime	= 0.00;
	m_strDatabaseDirectory = "";
}

CDeviceProperties::~CDeviceProperties()
{

}

void CDeviceProperties::ReadDeviceProperties()
{
	std::ifstream file("devices.txt");	// File to read device property data

	CDevices Devices;

	if (file.fail())	// Error handling
	{
		#ifdef _DEBUG
		OutputDebugString(TEXT("File failed to open | Not Found \n"));		// Debug output File not found/failed to open
		#endif
	}
	else
	{
		#ifdef _DEBUG
		OutputDebugString(TEXT("File found \n"));							// Debug output file found
		#endif

		int wordNum = 0;	// Tracks current word being read from file
		std::string line;	// Stored line from file text

		while (std::getline(file, line))	// While lines to read are availible
		{
			if (line != "")	// If line isn't blank, safeguarding the stoi function
			{
				switch (wordNum)	// Switch for word tracking
				{
				case 0:
					m_strScannerAName = Devices.TruncateHIDName(line);	// First line, Scanner A HID Name (truncated)
					break;
				case 1:
					m_strScannerBName = Devices.TruncateHIDName(line);	// Second line, Scanner B HID Name (truncated)
					break;
				case 2:
					m_strScannerLocation = line;						// Third line, Scanner Location
					break;
				case 3:
					m_iSpeedLimit = std::stoi(line.c_str());			// Fourth line, Speed limit of location
					break;
				case 4:
					m_dbScannerDistance = std::stof(line);				// Fifth line, Distance between scanners
					break;
				case 5:
					m_strDatabaseDirectory = line;						// Sixth line, Database name/directory
					break;
				}
				wordNum++;	// Increment line number/word number
			}
		}


		CalculateMaximumTravelTime();
		file.close();	// Close file once completed
	}
}

/// <summary>
/// Calculate expected travel time from speed limit (MPH) and scanner distance
/// (Scanner A & Scanner B - Metres)]
/// </summary>
void CDeviceProperties::CalculateMaximumTravelTime()
{
	double dbSpeedLimitMetres = m_iSpeedLimit * m_dbMPHToMiles;
	m_dbMaximumTravelTime = (m_dbScannerDistance / (dbSpeedLimitMetres / m_dbSecondsInHour));
	m_dbMaximumTravelTime += (m_dbMaximumTravelTime / 10);
}
