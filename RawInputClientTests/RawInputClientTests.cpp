#include "pch.h"
#include "CppUnitTest.h"
#include <chrono>
#include <thread>

#include "../RawInputClient/Persistence.h"
#include "../RawInputClient/Persistence.cpp"
#include "../RawInputClient/InputManager.h"
#include "../RawInputClient/InputManager.cpp"
#include "../RawInputClient/Vehicle.h"
#include "../RawInputClient/Vehicle.cpp"
#include "../RawInputClient/Registration.h"
#include "../RawInputClient/Registration.cpp"
#include "../RawInputClient/Clock.h"
#include "../RawInputClient/Clock.cpp"
#include "../RawInputClient/DatabaseHelper.h"
#include "../RawInputClient/DatabaseHelper.cpp"
#include "../RawInputClient/MenuCLI.h"
#include "../RawInputClient/MenuCLI.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RawInputClientTests
{
	TEST_CLASS(RawInputClientTests)
	{
	public:

		/// <summary>
		/// Test: The Registering a device, with two devices being recognised by the OS, ensuring the device is in the vector and it isn't recognised as a navigation device for CLI menus
		/// </summary>
		TEST_METHOD(RegisterDevice)
		{
			bool target = false;

			CPersistence::DeviceProperties *pDeviceSettings = new CPersistence::DeviceProperties();
			pDeviceSettings->m_vecstrRegisteredDevices.clear();
			pDeviceSettings->m_vecstrAllDevices.push_back("devA");
			pDeviceSettings->m_vecstrAllDevices.push_back("devB");
			pDeviceSettings->RegisterDevice(1, "devA");	// Selected index from UI of alldevices vector, and navigation device name passed through, can't add device names that are navigation devices

			if (std::find(pDeviceSettings->m_vecstrRegisteredDevices.begin(), pDeviceSettings->m_vecstrRegisteredDevices.end(), "devB") != pDeviceSettings->m_vecstrRegisteredDevices.end())
			{
				target = true;
			}

			pDeviceSettings = new CPersistence::DeviceProperties();
			Assert::IsTrue(target);
		}

		/// <summary>
		/// Test: The deregstering of a device, by adding a device to the vector and remove it with the function
		/// </summary>
		TEST_METHOD(DeregisterDevice)
		{
			bool target = false;

			CPersistence::DeviceProperties* pDeviceSettings = new CPersistence::DeviceProperties();
			pDeviceSettings->m_vecstrRegisteredDevices.clear();
			pDeviceSettings->m_vecstrRegisteredDevices.push_back("devA");	// Emulate a registered device being added
			pDeviceSettings->DeregisterDevice(0);

			if (std::find(pDeviceSettings->m_vecstrRegisteredDevices.begin(), pDeviceSettings->m_vecstrRegisteredDevices.end(), "devA") == pDeviceSettings->m_vecstrRegisteredDevices.end())
			{
				target = true;
			}

			pDeviceSettings = new CPersistence::DeviceProperties();
			Assert::IsTrue(target);
		}

		/// <summary>
		/// Test: Vehicle passing through safely
		/// </summary>
		TEST_METHOD(SafeVehicleDetection)
		{
			bool target = false;
			CInputManager* pInputManager = new CInputManager();
			pInputManager->pDevProp.m_dbOptimumTravelTime = 0.01;
			pInputManager->pDevProp.m_vecstrRegisteredDevices.clear();
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devA");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devB");
			pInputManager->pDevProp.m_strScannerLocation = " A1, Newcastle upon Tyne";
			pInputManager->pDevProp.m_strDatabaseDirectory = "..//..//DrivingOffences.accdb";

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			pInputManager->InputDetected("devA", 0, 'a');
			pInputManager->InputDetected("devA", 0, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			pInputManager->InputDetected("devB", 1, 'a');
			pInputManager->InputDetected("devB", 1, '\n');

			// Elapsed Time == 1 sec

			if (pInputManager->m_strVehicleOffence == "Safe")
			{
				target = true;
			}

			pInputManager = new CInputManager();
			Assert::IsTrue(target);
		}

		/// <summary>
		/// Test: Multiple time gates (I.E. 2+ scanning devices) testing if speed limit is breached
		/// </summary>
		TEST_METHOD(HighSpeedMultipleTimeGates)
		{
			bool target = false;

			CInputManager *pInputManager = new CInputManager();
			pInputManager->pDevProp.m_dbOptimumTravelTime = 2.00;
			pInputManager->pDevProp.m_vecstrRegisteredDevices.clear();
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devA");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devB");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devC");
			pInputManager->pDevProp.m_strScannerLocation = " A1, Newcastle upon Tyne";
			pInputManager->pDevProp.m_strDatabaseDirectory = "..//..//DrivingOffences.accdb";

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));	// Also checks that values aren't impacted by clocks current time and are "zeroed"
			pInputManager->InputDetected("devA", 0, 'a');
			pInputManager->InputDetected("devA", 0, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			pInputManager->InputDetected("devB", 1, 'a');
			pInputManager->InputDetected("devB", 1, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			pInputManager->InputDetected("devC", 2, 'a');
			pInputManager->InputDetected("devC", 2, '\n');

			// Time for vehicle is ~1.00sec, anything faster (lower) than this will be speeding

			if (pInputManager->m_strVehicleOffence == "Speeding")
			{
				target = true;
			}

			pInputManager = new CInputManager();
			Assert::IsTrue(target);
		}

		/// <summary>
		/// Test: Multiple time gates (I.E. 2+ scanning devices) testing if speed limit is breached in reverse
			/// </summary>
		TEST_METHOD(HighSpeedMultipleTimeGatesWrongOrder)
		{
			bool target = false;

			CInputManager* pInputManager = new CInputManager();
			pInputManager->pDevProp.m_dbOptimumTravelTime = 2.00;
			pInputManager->pDevProp.m_vecstrRegisteredDevices.clear();
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devA");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devB");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devC");
			pInputManager->pDevProp.m_strScannerLocation = " A1, Newcastle upon Tyne";
			pInputManager->pDevProp.m_strDatabaseDirectory = "..//..//DrivingOffences.accdb";


			std::this_thread::sleep_for(std::chrono::milliseconds(2000));	// Also checks that values aren't impacted by clocks current time and are "zeroed"
			pInputManager->InputDetected("devC", 2, 'a');
			pInputManager->InputDetected("devC", 2, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			pInputManager->InputDetected("devB", 1, 'a');
			pInputManager->InputDetected("devB", 1, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));	
			pInputManager->InputDetected("devA", 0, 'a');
			pInputManager->InputDetected("devA", 0, '\n');

			// Time for vehicle is ~1.00sec, anything faster (lower) than this will be speeding

			if (pInputManager->m_strVehicleOffence == "Speeding & Wrong Way")
			{
				target = true;
			}

			pInputManager = new CInputManager();
			Assert::IsTrue(target);
		}


		/// <summary>
		/// Test: If high speed is detected through gate
		/// </summary>
		TEST_METHOD(HighSpeedDetected)
		{
			bool target = false;
			CInputManager *pInputManager = new CInputManager();
			pInputManager->pDevProp.m_dbOptimumTravelTime = 2.00;
			pInputManager->pDevProp.m_vecstrRegisteredDevices.clear();
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devA");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devB");
			pInputManager->pDevProp.m_strScannerLocation = " A1, Newcastle upon Tyne";
			pInputManager->pDevProp.m_strDatabaseDirectory = "..//..//DrivingOffences.accdb";

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			pInputManager->InputDetected("devA", 0,'a');
			pInputManager->InputDetected("devA", 0, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			pInputManager->InputDetected("devB", 1, 'a');
			pInputManager->InputDetected("devB", 1, '\n');

			// Elapsed Time == 1 sec

			if (pInputManager->m_strVehicleOffence == "Speeding")
			{
				target = true;
			}

			pInputManager = new CInputManager();
			Assert::IsTrue(target);
		}

		/// <summary>
		/// Test: If wrong way is detected
		/// </summary>
		TEST_METHOD(WrongWayDetected)
		{
			bool target = false;
			CInputManager *pInputManager = new CInputManager();
			pInputManager->pDevProp.m_dbOptimumTravelTime = 0.1;
			pInputManager->pDevProp.m_vecstrRegisteredDevices.clear();
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devA");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devB");
			pInputManager->pDevProp.m_strScannerLocation = " A1, Newcastle upon Tyne";
			pInputManager->pDevProp.m_strDatabaseDirectory = "..//..//DrivingOffences.accdb";

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			pInputManager->InputDetected("devB", 1, 'a');
			pInputManager->InputDetected("devB", 1, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			pInputManager->InputDetected("devA", 0, 'a');
			pInputManager->InputDetected("devA", 0, '\n');

			// Elapsed Time == 1 sec

			if (pInputManager->m_strVehicleOffence == "Wrong Way")
			{
				target = true;
			}

			pInputManager = new CInputManager();
			Assert::IsTrue(target);
		}

		/// <summary>
		/// Test: Wrong way (negative time) and high speed is detected
		/// </summary>
		TEST_METHOD(WrongWayHighSpeedDetected)
		{
			bool target = false;
			CInputManager *pInputManager = new CInputManager();
			pInputManager->pDevProp.m_dbOptimumTravelTime = 2.00;
			pInputManager->pDevProp.m_vecstrRegisteredDevices.clear();
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devA");
			pInputManager->pDevProp.m_vecstrRegisteredDevices.push_back("devB");
			pInputManager->pDevProp.m_strScannerLocation = " A1, Newcastle upon Tyne";
			pInputManager->pDevProp.m_strDatabaseDirectory = "..//..//DrivingOffences.accdb";

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			pInputManager->InputDetected("devB", 1, 'a');
			pInputManager->InputDetected("devB", 1, '\n');
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			pInputManager->InputDetected("devA", 0, 'a');
			pInputManager->InputDetected("devA", 0, '\n');

			// Elapsed Time == 1 sec

			if (pInputManager->m_strVehicleOffence == "Speeding & Wrong Way")
			{
				target = true;
			}

			pInputManager = new CInputManager();
			Assert::IsTrue(target);
		}
	};
}