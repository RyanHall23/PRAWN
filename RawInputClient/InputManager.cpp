#include "InputManager.h"

CInputManager::CInputManager()
{

}

CInputManager::~CInputManager()
{

}

/// <summary>
/// On Keyboard in put detected, with parameters for device name and pressed key (char)
/// </summary>
/// <param name="strShortDeviceName"></param>
/// <param name="cRecievedKey"></param>
void CInputManager::InputDetected(std::string strShortDeviceName, int iDeviceIndex, unsigned char cRecievedKey)
{
    std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

    // Check if registration plate is null
    std::string strRegistrationPlate = pRegistration.BuildRegistration(cRecievedKey, iDeviceIndex, pPersistence->m_vecstrRegisteredDevices.size());

    if (strRegistrationPlate == NULLSTRING)
    {
        return;
    }

    #ifdef _DEBUG
    OutputDebugString((LPCSTR)strShortDeviceName.c_str());   // Debug output device name
    OutputDebugString(" Plate : ");

    OutputDebugString((LPCSTR)strRegistrationPlate.c_str());        // Debug output built registration 
    OutputDebugString("\n");
    #endif

    CVehicle *pVehicle = new CVehicle();
    pVehicle->m_strRegistration = strRegistrationPlate;
    pVehicle->m_vecDbScanningTimes.resize(pPersistence->m_vecstrRegisteredDevices.size());


    if (VehicleExists(pVehicle))
    {
        auto aGetVehicle = GetVehicle(pVehicle->m_strRegistration);    // Get index and vehicle ptr
        int iVehicleIndex;

        if (std::get<0>(aGetVehicle) != NULL)
        {
            pVehicle = std::get<0>(aGetVehicle);
            iVehicleIndex = std::get<1>(aGetVehicle);
            // check if in array cache
            // check vehicle
            // add to array cache
            // purge array cache daily
            if (pVehicle->m_iDirectionOrigin != iDeviceIndex)   // Edge case : Vehicle passes back over origin scanner, not reaching both scanners
            {
                pVehicle->m_vecDbScanningTimes.at(iDeviceIndex) = pClock.GetTime();
                SetVehicle(pVehicle, iVehicleIndex);    // Set vehicle with new timings

                if (std::all_of(pVehicle->m_vecDbScanningTimes.begin(), pVehicle->m_vecDbScanningTimes.end(), [](double i) {return i > 0.00; }))
                {
                    CheckVehicle(pVehicle);
                    //OutputDebugString(" None are empty "); // Run check here, all vector indexes are filled meaning each scanner has been hit
                }
            }
            else
            {
                RemoveVehicle(pVehicle);    // Remove vehicle, assmued to pass back over initial entrance point and leave PRAWN "circuit"
            }
        }
    }
    else
    {
        pVehicle->m_iDirectionOrigin = iDeviceIndex;        // Used passed deviceIndex to set origin (Used for direction)            
        pVehicle->m_vecDbScanningTimes.at(iDeviceIndex) = pClock.GetTime(); // Use deviceIndex to set timestamp at correct vector index to ensure all indexes are filled (sequentially)
        AddVehicle(pVehicle);   // Add vehicle to "circuit"
    }
}

/// <summary>
/// Check if vehicle is in a legal or illegal state after passing through scanners
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::CheckVehicle(CVehicle *vVehicle)
{
    CalculateTotalTravelTime(vVehicle); // Calculate to travel time from all gates

    m_strVehicleOffence = "Safe";   // Assign as "SAFE" unless following conditions are satisfied

    if (vVehicle->m_iDirectionOrigin == 0)    // If heading in right direction
    {
        if (vVehicle->m_dbTotalTravelTime < pDevProp.m_dbOptimumTravelTime)    // If travel time is lower than minimum (Illegal)
        {
            #ifdef _DEBUG
            OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output device name
            OutputDebugString(" Speeding \n");
            #endif

            m_strVehicleOffence = "Speeding";
            pDatabaseHelper.UpdateDatabase(vVehicle->m_strRegistration, m_strVehicleOffence, pDevProp.m_strScannerLocation, pDevProp.m_strDatabaseDirectory);
            RemoveVehicle(vVehicle);    // Delete call
        }
    }
    else if(vVehicle->m_iDirectionOrigin == vVehicle->m_vecDbScanningTimes.size() - 1)  // If started from last possible location
    {
        if (vVehicle->m_dbTotalTravelTime < pDevProp.m_dbOptimumTravelTime)    // If travel time is lower than minimum (Illegal)
        {
            #ifdef _DEBUG
            OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output device name
            OutputDebugString(" Speeding & Wrong Way \n");
            #endif

            m_strVehicleOffence = "Speeding & Wrong Way";
            pDatabaseHelper.UpdateDatabase(vVehicle->m_strRegistration, m_strVehicleOffence, pDevProp.m_strScannerLocation, pDevProp.m_strDatabaseDirectory);
            RemoveVehicle(vVehicle);    // Delete call

        }
        else // No speed limit broken, wrong way is passed
        {
            #ifdef _DEBUG
            OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output device name
            OutputDebugString(" Wrong Way \n");
            #endif

            m_strVehicleOffence = "Wrong Way";
            pDatabaseHelper.UpdateDatabase(vVehicle->m_strRegistration, m_strVehicleOffence, pDevProp.m_strScannerLocation, pDevProp.m_strDatabaseDirectory);
            RemoveVehicle(vVehicle);    // Delete call
        }
    }
}

/// <summary>
/// Calculate total travel time from gate to gate for vehicle for legality comparisons
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::CalculateTotalTravelTime(CVehicle* vVehicle)
{
    double dbZeroTime;  // Used to zero times to calculate an accurate average

    if (vVehicle->m_vecDbScanningTimes.size() > 2)  // If there are more than two scanners (An average can be calculated) Find AVERAGE MEAN of TIMES
    {
        if (vVehicle->m_iDirectionOrigin == 0)
        {
            dbZeroTime = vVehicle->m_vecDbScanningTimes.at(0);  // Get "0" timestamp

            for (int i = 0; i < vVehicle->m_vecDbScanningTimes.size(); ++i)
            {
                vVehicle->m_vecDbScanningTimes.at(i) -= dbZeroTime;                     // Calculate all times to normalise data       
                vVehicle->m_dbTotalTravelTime += vVehicle->m_vecDbScanningTimes.at(i);  // Total amount of times
            }

            vVehicle->m_dbTotalTravelTime /= vVehicle->m_vecDbScanningTimes.size(); // Find MEAN from amount of scanners
        }
        else
        {
            dbZeroTime = vVehicle->m_vecDbScanningTimes.at(vVehicle->m_vecDbScanningTimes.size()-1);    // Get negative "0" timestamp

            for (int i = 0; i < vVehicle->m_vecDbScanningTimes.size(); ++i)
            {
                vVehicle->m_vecDbScanningTimes.at(i) -= dbZeroTime;                     // Calculate all times to normalise data
                vVehicle->m_dbTotalTravelTime -= vVehicle->m_vecDbScanningTimes.at(i);  // Total amount of times (negative)
            }

            vVehicle->m_dbTotalTravelTime /= vVehicle->m_vecDbScanningTimes.size(); // Find MEAN from amount of scanners
        }

    }
    else 
    {
        if (vVehicle->m_iDirectionOrigin == 0)
        {
            vVehicle->m_dbTotalTravelTime = vVehicle->m_vecDbScanningTimes.at(1) - vVehicle->m_vecDbScanningTimes.at(0);    // If two scanners do standard end - start calculation
        }
        else
        {
            vVehicle->m_dbTotalTravelTime = vVehicle->m_vecDbScanningTimes.at(0) - vVehicle->m_vecDbScanningTimes.at(1);    // If two scanners do standard end - start negative (reverse) calculation
        }
    }
}

/// <summary>
/// Add a vehicle to the active vehicles vector
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::AddVehicle(CVehicle *vVehicle)
{
    if (!VehicleExists(vVehicle))   // If vehicle doesn't exist/active
    {
        m_vecActiveVehicles.push_back(vVehicle);
        #ifdef _DEBUG
        OutputDebugString("\n");
        OutputDebugString("New Vehicle: ");
        OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output new vehicle
        OutputDebugString("\n");
        #endif
    }
    else
    {
        #ifdef _DEBUG
        OutputDebugString("\n");
        OutputDebugString("Vehicle Exists: ");
        OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output new vehicle
        OutputDebugString("\n");
        #endif
    }
}

/// <summary>
/// Remove a vehicle from the active vehicles vector, safely
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::RemoveVehicle(CVehicle *vVehicle)
{
    for (unsigned int i = 0; i < m_vecActiveVehicles.size(); ++i)
    {
        if (vVehicle->m_strRegistration == m_vecActiveVehicles.at(i)->m_strRegistration)
        {
            // Delete safely here
            #ifdef _DEBUG
            OutputDebugString("\n");
            OutputDebugString("Deleting Vehicle: ");
            OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output new vehicle
            OutputDebugString("\n");
            #endif

            delete m_vecActiveVehicles.at(i);
            m_vecActiveVehicles.erase(m_vecActiveVehicles.begin() +i);

        }
    }
}

/// <summary>
/// Called from a thread to purge vehicles that have been in the collection of active vehicles for more than 10 times their optimum travel time
/// </summary>
void CInputManager::PurgeVehicles()
{
    std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

    while (true) 
    {
        for (unsigned int i = 0; i < m_vecActiveVehicles.size(); ++i)
        {
            if (pClock.GetTime() - m_vecActiveVehicles.at(i)->m_vecDbScanningTimes.at(0) > (pPersistence->m_dbOptimumTravelTime * M_I_PURGEFACTOR))
            {
                RemoveVehicle(m_vecActiveVehicles.at(i));   
                // Works by finding time elapsed between hitting first scanner and current time, if it's larger then purge time
                // Need to add implementation for n Scanner being hit first
                
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(M_I_THREADSLEEPSECONDS));
    }
}

/// <summary>
/// Check if vehicle already exists in active vehicle vector
/// </summary>
/// <param name="vVehicle"></param>
/// <returns></returns>
bool CInputManager::VehicleExists(CVehicle *vVehicle)
{
    for (unsigned int i = 0; i < m_vecActiveVehicles.size(); ++i)
    {
        if (vVehicle->m_strRegistration == m_vecActiveVehicles.at(i)->m_strRegistration)
        {
            return true;
        }
    }
    return false;   // If not present, return false
}

/// <summary>
/// Get vehicle type by searching for registration plate (will always be present) and return it
/// </summary>
/// <param name="strRegistrationPlate"></param>
/// <returns></returns>
std::tuple<CVehicle*, int> CInputManager::GetVehicle(std::string strRegistrationPlate)
{
    for (unsigned int i = 0; i < m_vecActiveVehicles.size(); ++i)
    {
        if (strRegistrationPlate == m_vecActiveVehicles.at(i)->m_strRegistration)
        {
            return std::tuple<CVehicle*, int>{m_vecActiveVehicles.at(i), i};
        }
    }

    return std::tuple<CVehicle*, int>{NULL, 0};
}

/// <summary>
/// Set a vehicle in the active vehicles vector, from being partially complete on entry
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::SetVehicle(CVehicle* vVehicle, int iVecIndex)
{
    m_vecActiveVehicles.at(iVecIndex) = vVehicle;
}

/// <summary>
/// Validate MOT/TAX on reg plate
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::ValidateVehicle(CVehicle* vVehicle)
{
    //pVesCheck.
}