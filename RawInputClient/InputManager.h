#pragma once

#include "Registration.h"
#include "Persistence.h"
#include "Vehicle.h"
#include "Clock.h"

#include <windows.h>
#include <atlstr.h>

#using <System.Data.dll>
#using <System.dll>

#include <string>
#include <vector>
#include <tuple>
#include <thread>

using namespace System::Data::OleDb;

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	void InputDetected(std::string deviceName, unsigned char RecievedKey);
	int CheckScannerIsRegistered(std::string strShortDeviceName);
	void CheckVehicle(CVehicle *vVehicle);
	void AddVehicle(CVehicle *vVehicle);
	void RemoveVehicle(CVehicle *vVehicle);
	bool VehicleExists(CVehicle *vVehicle);
	std::tuple<CVehicle*, int> GetVehicle(std::string strRegistrationPlate);
	void SetVehicle(CVehicle *vVehicle, int iVecIndex);
	void UpdateDatabase(std::string strRegistrationPlate, std::string Offence);
	void PurgeVehicles();

	std::vector<CVehicle*> m_vecActiveVehicles;

	CRegistration pRegistration;					// To Do: Create smart pointer of Registration class
	CClock pClock;

private:
	int const			M_I_PURGEFACTOR = 10;
	int const			M_I_THREADSLEEPSECONDS = 10;
	const std::string	NULLSTRING = "";
};