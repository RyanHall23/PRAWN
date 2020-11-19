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
void CInputManager::InputDetected(std::string strShortDeviceName, unsigned char cRecievedKey)
{
    #ifdef _DEBUG
    OutputDebugString((LPCSTR)strShortDeviceName.c_str());   // Debug output device name
    OutputDebugString(" Key: ");

    std::string strOutputInKey(1, cRecievedKey);

    OutputDebugString((LPCSTR)strOutputInKey.c_str());   // Debug output entered key
    OutputDebugString("\n");
    #endif

    std::string strRegistrationPlate = pRegistration.BuildRegistration(cRecievedKey);
    if (strRegistrationPlate == NULLSTRING)
    {
        return;
    }

    // Get current device index in vector for comparisons, and stop errors if device is not registered
    int iCurrentDeviceIndex;
    if (iCurrentDeviceIndex = CheckScannerIsRegistered(strShortDeviceName) < 0)
    {
        return;
    }

    #ifdef _DEBUG
    OutputDebugString((LPCSTR)strRegistrationPlate.c_str());        // Debug output built registration 
    OutputDebugString("\n");
    #endif

    CVehicle *pVehicle = new CVehicle();
    int iVehicleIndex;

    pVehicle->m_strRegistration = strRegistrationPlate;

    std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

    if (VehicleExists(pVehicle))
    {
        auto aGetVehicle = GetVehicle(strRegistrationPlate);    // Get index and vehicle ptr
        if (std::get<0>(aGetVehicle) != NULL)
        {
            pVehicle = std::get<0>(aGetVehicle);
            iVehicleIndex = std::get<1>(aGetVehicle);

            if (pVehicle->m_iDirectionOrigin != iCurrentDeviceIndex)// Edge case : Vehicle passes back over origin scanner, not reaching both scanners
            {
                pVehicle->m_dbEndTime = pClock.GetTime();
                pVehicle->m_dbTotalTravelTime = pVehicle->m_dbEndTime - pVehicle->m_dbStartTime;
                SetVehicle(pVehicle, iVehicleIndex);
            }
            else
            {
                RemoveVehicle(pVehicle);
            }
        }
    }
    else
    {
        for (int i = 0; i < pPersistence->m_vecstrSelectedDevices.size(); ++i)
        {
            if (pPersistence->m_vecstrSelectedDevices.at(i) == strShortDeviceName)
            {
                pVehicle->m_iDirectionOrigin = i;
                pVehicle->m_dbStartTime = pClock.GetTime();
            }
        }

        AddVehicle(pVehicle);
    }
}

/// <summary>
/// Checks if a scanner is registered (Edge case of an incorrect device passing the raw input check) to stop any collection overflow errors
/// Also gets the index of the scanner (origin key) for comparisons
/// </summary>
/// <param name="strShortDeviceName"></param>
/// <returns></returns>
int CInputManager::CheckScannerIsRegistered(std::string strShortDeviceName)
{
    std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());

    for (int i = 0; i < pPersistence->m_vecstrSelectedDevices.size(); ++i)
    {
        if (pPersistence->m_vecstrSelectedDevices.at(i) == strShortDeviceName)
        {
            return i;
        }
    }

    // If scanner is not found in registered devices
    return -1;
}

/// <summary>
/// Check if vehicle is in a legal or illegal state after passing through scanners
/// </summary>
/// <param name="vVehicle"></param>
void CInputManager::CheckVehicle(CVehicle *vVehicle)
{
//    if (vVehicle->m_strDirectionOrigin == "A")    // If heading in right direction
//    {
//        if (vVehicle->m_dbTotalTravelTime < pDevices.m_dbOptimumTravelTime)    // If travel time is lower than minimum (Illegal)
//        {
//            #ifdef _DEBUG
//            OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output device name
//            OutputDebugString(" Speeding \n");
//            #endif
//
//            // Database call
//            UpdateDatabase(vVehicle->m_strRegistration, "Speeding");
//            // Delete call
//            RemoveVehicle(vVehicle);
//        }
//    }
//    else if(vVehicle->m_strDirectionOrigin == "B")  // If started from point B (Wrong direction
//    {
//        if (vVehicle->m_dbTotalTravelTime < pDevices.m_dbOptimumTravelTime)    // If travel time is lower than minimum (Illegal)
//        {
//            #ifdef _DEBUG
//            OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output device name
//            OutputDebugString(" Speeding & Wrong Way \n");
//            #endif
//
//            // Database call
//            UpdateDatabase(vVehicle->m_strRegistration, "Speeding & Wrong Way");
//            // Delete call
//            RemoveVehicle(vVehicle);
//        }
//        else // No speed limit broken, wrong way is passed
//        {
//            #ifdef _DEBUG
//            OutputDebugString((LPCSTR)vVehicle->m_strRegistration.c_str());   // Debug output device name
//            OutputDebugString(" Wrong Way \n");
//            #endif
//
//            // Database call
//            UpdateDatabase(vVehicle->m_strRegistration, "Wrong Way");
//            // Delete call
//            RemoveVehicle(vVehicle);
//        }
//    }
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
            if (pClock.GetTime() - m_vecActiveVehicles.at(i)->m_dbStartTime > (pPersistence->m_dbOptimumTravelTime * M_I_PURGEFACTOR))
            {
                RemoveVehicle(m_vecActiveVehicles.at(i));
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
void CInputManager::SetVehicle(CVehicle *vVehicle, int iVecIndex)
{
    m_vecActiveVehicles.at(iVecIndex) = vVehicle;
    //CheckVehicle(vVehicle);
}

/// <summary>
/// Update Database with new entry if required from illegal vehicle state data
/// 
/// Reference of OLEDB - https://docs.microsoft.com/en-us/dotnet/api/system.data.sqlclient.sqlconnection.begintransaction?view=netframework-4.7.2
/// 64-bit Database Engine - https://www.microsoft.com/en-us/download/confirmation.aspx?id=13255
/// </summary>
void CInputManager::UpdateDatabase(std::string strRegistrationPlate, std::string strOffence)
{
    OleDbConnection^ oleConnection = nullptr;
    OleDbCommand^ oleCommand = nullptr;
    OleDbDataReader^ dbReader = nullptr;
    std::unique_ptr<CPersistence::DeviceProperties> pPersistence(new CPersistence::DeviceProperties());


    std::string prepSQL = ("INSERT INTO tblOffences (Registration, Location, Offence) VALUES ('" + strRegistrationPlate + "','" + pPersistence->m_strScannerLocation + "','" + strOffence + "')");

    System::String^ result = gcnew System::String(prepSQL.c_str());
    System::String^ strSQL = gcnew System::String(result);
    System::String^ sstrDatabaseDirectory = gcnew System::String(pPersistence->m_strDatabaseDirectory.c_str()); // Convert std::string to System::String

    try
    {
        // TODO: Use db locatio from DeviceProperties and make it dynamically modifiable in runtime
        oleConnection = gcnew OleDbConnection("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=..\\DrivingOffences.accdb");
        oleConnection->Open();
        oleCommand = gcnew OleDbCommand(strSQL, oleConnection);

        dbReader = oleCommand->ExecuteReader(System::Data::CommandBehavior::CloseConnection);
    }
    catch (System::Exception^ ex)
    {
        CString cstrException = ex->ToString();
        #ifdef _DEBUG
        OutputDebugString("\n");
        OutputDebugString(cstrException);
        OutputDebugString("\n");
        #endif
    }

    delete oleConnection;
    delete oleCommand;
    delete dbReader;
}