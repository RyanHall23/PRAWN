#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <algorithm>

#include "Registration.h"
#include "Persistence.h"
#include "DatabaseHelper.h"
#include "VESCheck.h"
#include "Vehicle.h"
#include "Clock.h"

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	void InputDetected(std::string deviceName, int deviceIndex, unsigned char RecievedKey);
	void CheckVehicle(CVehicle* vVehicle);
	void CalculateTotalTravelTime(CVehicle* vVehicle);
	void AddVehicle(CVehicle* vVehicle);
	void RemoveVehicle(CVehicle* vVehicle);
	bool VehicleExists(CVehicle* vVehicle);
	std::tuple<CVehicle*, int> GetVehicle(std::string strRegistrationPlate);
	void SetVehicle(CVehicle* vVehicle, int iVecIndex);
	void ValidateVehicle(CVehicle* vVehicle);
	void PurgeVehicles();

	std::vector<CVehicle*> m_vecActiveVehicles;

	// To Do: Create smart pointers
	CPersistence::DeviceProperties pDevProp;
	CRegistration pRegistration;					
	CClock pClock;
	CDatabaseHelper pDatabaseHelper;
	CVESCheck pVesCheck;

	std::string m_strVehicleOffence;	// Used for testing vehicle outcome through circuit
private:
	int const			M_I_PURGEFACTOR = 10;
	int const			M_I_THREADSLEEPSECONDS = 10;
	const std::string	NULLSTRING = "";
};