#include "Devices.h"


CDevices::CDevices()
{
}

CDevices::~CDevices()
{

}

CString CDevices::TruncateHIDName(std::string devName)
{
	if (devName.size() < 19)	// Valid keyboard length including PID, VID & Bus data
	{
		CString cstrErrorMsg = "Device Name Error: ";
		return cstrErrorMsg;
	}

	// TODO: Make this dynamic
	int iStart	= 8;		// Start point of truncating string
	int iEnd	= 17;		// End point of truncating string

	std::string strTruncated = devName.substr(iStart, iEnd);	// Truncates device name to following params

	return strTruncated.c_str();	// Return truncated devicename
}