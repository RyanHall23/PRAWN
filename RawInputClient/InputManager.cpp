#include "InputManager.h"


CInputManager::CInputManager()
{
    pDeviceProperties.ReadDeviceProperties();
}

CInputManager::~CInputManager()
{
}

/// <summary>
/// On Keyboard in put detected, with parameters for device name and pressed key (char)
/// </summary>
/// <param name="strShortDeviceName"></param>
/// <param name="cRecievedKey"></param>
void CInputManager::InputDetected(std::string strShortDeviceName, unsigned char cRecievedKey)
{
    // TODO: Handling and registration building
    std::string cFilteredKey;
    std::string strRegistrationPlate;

    CVehicle *pVehicle = new CVehicle();
    int iVehicleIndex;

    if (isdigit(cRecievedKey) || isalpha(cRecievedKey))                 // Check for valid alphanumeric key
    {
        cFilteredKey = std::toupper(cRecievedKey);                      // Convert to Upper case to avoid mixing cases
        strRegistrationPlate = pRegistration.BuildRegistration(cFilteredKey);
    }
    else if (cRecievedKey == pDevices.m_cReturnKeyVirtualCode)          // Check if Return Key/End of string key has been entered
    {
        cFilteredKey = pDevices.m_strReturnKeyMessage;                  // Mark with '!'
        strRegistrationPlate = pRegistration.BuildRegistration(cFilteredKey);

        #ifndef DEBUG
        OutputDebugString((LPCSTR)strRegistrationPlate.c_str());        // Debug output built registration 
        OutputDebugString(" ");
        #endif

        pVehicle->m_strRegistration = strRegistrationPlate;

        if (VehicleExists(pVehicle))
        {
            auto aGetVehicle = GetVehicle(strRegistrationPlate);    // Get index and vehicle ptr
            pVehicle = std::get<0>(aGetVehicle);
            iVehicleIndex = std::get<1>(aGetVehicle);

            pVehicle->m_dbEndTime = pClock.GetTime();
            pVehicle->m_dbTotalTravelTime = pVehicle->m_dbEndTime - pVehicle->m_dbStartTime;
            SetVehicle(pVehicle, iVehicleIndex);
        }
        else
        {
            if (strShortDeviceName == pDeviceProperties.m_strScannerAName)
            {
                pVehicle->m_strDirectionOrigin = "A";
                pVehicle->m_dbStartTime = pClock.GetTime();
            }
            else if (strShortDeviceName == pDeviceProperties.m_strScannerBName)
            {
                pVehicle->m_strDirectionOrigin = "B";
                pVehicle->m_dbStartTime = pClock.GetTime();
            }
            else // Debugging Keyboard Input (Only Forward directions)
            {
                pVehicle->m_strDirectionOrigin = "A";
                pVehicle->m_dbStartTime = pClock.GetTime();
            }

            AddVehicle(pVehicle);
        }
    }


    #ifndef DEBUG
    OutputDebugString((LPCSTR)strShortDeviceName.c_str());   // Debug output device name
    OutputDebugString(" ");

    OutputDebugString((LPCSTR)cFilteredKey.c_str());   // Debug output entered key
    OutputDebugString("\n");
    #endif
}

/// <summary>
/// Check if vehicle is in a legal or illegal state after passing through scanners
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::CheckVehicle(CVehicle *vVehicle)
{
    // If speeding etc
    // Then RemoveVehicle
    if (vVehicle->m_strDirectionOrigin == "A")    // If heading in right direction
    {
        if (vVehicle->m_dbTotalTravelTime < pDeviceProperties.m_fMaximumTravelTime)    // If travel time is lower than minimum (Illegal)
        {
            // Database call
            UpdateDatabase(vVehicle->m_strRegistration, "Speeding");
            // Delete call
            RemoveVehicle(vVehicle);
        }
    }
    else
    {

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
        #ifndef DEBUG
        OutputDebugString("\n");
        OutputDebugString("New Vehicle: ");
        OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output new vehicle
        OutputDebugString("\n");
        #endif
    }
    else
    {
        OutputDebugString("\n");
        OutputDebugString("Vehicle Exists: ");
        OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output new vehicle
        OutputDebugString("\n");
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
            delete m_vecActiveVehicles.at(i);
            m_vecActiveVehicles.erase(m_vecActiveVehicles.begin() +i);
        }
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
}

/// <summary>
/// Set a vehicle in the active vehicles vector, from being partially complete on entry
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::SetVehicle(CVehicle *vVehicle, int iVecIndex)
{
    m_vecActiveVehicles.at(iVecIndex) = vVehicle;
    CheckVehicle(vVehicle);
}

/// <summary>
/// Update Database with new entry if required from illegal vehicle state data
/// </summary>
void CInputManager::UpdateDatabase(std::string strRegistrationPlate, std::string Offence)
{

}