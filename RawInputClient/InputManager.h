#pragma once

#include <windows.h>
#include <atlstr.h>
#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <algorithm>

#include "Registration.h"
#include "Persistence.h"
#include "Vehicle.h"
#include "Clock.h"

//#using <System.Data.dll>
//#using <System.dll>

//using namespace System::Data::OleDb;

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
	void PurgeVehicles();

	std::vector<CVehicle*> m_vecActiveVehicles;

	CPersistence::DeviceProperties pDevProp;
	CRegistration pRegistration;					// To Do: Create smart pointer of Registration class
	CClock pClock;

	std::string m_strVehicleOffence;	// Used for testing vehicle outcome through circuit
private:
	int const			M_I_PURGEFACTOR = 10;
	int const			M_I_THREADSLEEPSECONDS = 10;
	const std::string	NULLSTRING = "";
};