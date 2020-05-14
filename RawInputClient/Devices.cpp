#include "Devices.h"

CDevices::CDevices()
{
}

CDevices::~CDevices()
{

}

std::string CDevices::TruncateHIDName(std::string deviceName)
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