#include "Persistence.h"

// Static initialisations
std::vector<std::string>	CPersistence::DeviceProperties::m_vecstrAllDevices;			// Vector of all availible devices
std::vector<std::string>	CPersistence::DeviceProperties::m_vecstrSelectedDevices;	// Vector of selected devices in loop
std::string					CPersistence::DeviceProperties::m_strScannerLocation;		// Scanner Location
int							CPersistence::DeviceProperties::m_iSpeedLimit;				// Speed limit at location				(MPH)
double						CPersistence::DeviceProperties::m_dbScannerDistance;		// Distance between scanners			(Metres)
double						CPersistence::DeviceProperties::m_dbOptimumTravelTime;		// Expected Travel time between scanners
std::string					CPersistence::DeviceProperties::m_strDatabaseDirectory;		// Database location on disc & name

CPersistence::CPersistence()
{

}

CPersistence::~CPersistence()
{

}

std::unique_ptr<CPersistence::DeviceProperties> CPersistence::ReadSettings()
{
	std::ifstream file("devices.txt");	// File to read device property data

	std::unique_ptr<CPersistence::DeviceProperties> newConfig(new CPersistence::DeviceProperties());

	if (file.fail())	// Error handling
	{
		#ifdef _DEBUG
		OutputDebugString(TEXT("File failed to open | Not Found \n"));		// Debug output File not found/failed to open
		return newConfig;
		#endif
	}
	else
	{
		#ifdef _DEBUG
		OutputDebugString(TEXT("File found \n"));	// Debug output file found
		#endif

		int wordNum = 0;							// Tracks current word being read from file
		std::string line;							// Stored line from file text

		while (std::getline(file, line))			// While lines to read are availible
		{
			if (line != "")							// If line isn't blank, safeguarding the stoi function
			{
				switch (wordNum)					// Switch for word tracking
				{
				case 0:
					newConfig->m_iSpeedLimit = std::stoi(line.c_str());				// First line, Speed limit of location
					break;
				case 1:
					newConfig->m_dbScannerDistance = std::stof(line);				// Second line, Distance between scanners
					break;
				case 2:
					newConfig->m_strScannerLocation = line;							// third line, Distance between scanners
					break;
				case 3:
					newConfig->m_strDatabaseDirectory = line;						// Fourth line, Database name/directory
					break;
				case 4:																// Fifth line, Process amount of scanners connected
					int temp = std::stoi(line.c_str());		// Get amount of devices number
					std::getline(file, line);				// Increment line number
					for (int i = 0; i < temp; ++i)			// For as l
					{
						if (line == "")
						{
							std::getline(file, line);
							newConfig->m_vecstrSelectedDevices.push_back(line);
							std::getline(file, line);
						}
						else
						{
							newConfig->m_vecstrSelectedDevices.push_back(line);
						}
					}
				}
				wordNum++;	// Increment line number/word number
			}
		}


		newConfig->CalculateMaximumTravelTime();
		file.close();	// Close file once completed
		return newConfig;
	}
}

 //void CPersistence::SaveSettings(CDevices devicesConfig)
 //{

 //}

std::string CPersistence::TruncateHIDName(std::string deviceName)
{
	if (deviceName.size() < 19)	// Valid keyboard length including PID, VID & Bus data
	{
		std::string strErrorMsg = "Device Name Error: ";
		return strErrorMsg;
	}

	// TODO: Make this dynamic
	int iStart	= 8;		// Start point of truncating string
	int iEnd	= 17;		// End point of truncating string

	std::string strTruncated = deviceName.substr(iStart, iEnd);	// Truncates device name to following params

	return strTruncated.c_str();	// Return truncated devicename
}
