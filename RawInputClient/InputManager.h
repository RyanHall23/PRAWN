#pragma once

#include "Registration.h"
#include "DeviceProperties.h"
#include "Devices.h"
#include "Vehicle.h"
#include "Clock.h"

#include <windows.h>
#include <string>
#include <vector>
#include <tuple>

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	void InputDetected(std::string deviceName, unsigned char RecievedKey);
	void CheckVehicle(CVehicle *vVehicle);
	void AddVehicle(CVehicle *vVehicle);
	void RemoveVehicle(CVehicle *vVehicle);
	bool VehicleExists(CVehicle *vVehicle);
	std::tuple<CVehicle*, int> GetVehicle(std::string strRegistrationPlate);
	void SetVehicle(CVehicle *vVehicle, int iVecIndex);
	void UpdateDatabase(std::string strRegistrationPlate, std::string Offence);

	std::vector<CVehicle*> m_vecActiveVehicles;

	CDevices pDevices;						// To Do: Create smart pointer of Devices class
	CRegistration pRegistration;			// To Do: Create smart pointer of Registration class
	CDeviceProperties pDeviceProperties;	// To Do: Create smart pointer of Registration class
	CClock pClock;

};

