#pragma once

#include "Registration.h"
#include "Devices.h"
#include "Vehicle.h"

#include <windows.h>
#include <string>
#include <vector>

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	void InputDetected(std::string deviceName, unsigned char RecievedKey);
	void CheckVehicle();
	void AddVehicle();
	void RemoveVehicle();

	static std::vector<CVehicle> m_vecActiveVehicles;

	CDevices pDevices;				// To Do: Create smart pointer of Devices class
	CRegistration pRegistration;	// To Do: Create smart pointer of Registration class 

};

