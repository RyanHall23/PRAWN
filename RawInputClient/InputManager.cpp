#include "InputManager.h"


CInputManager::CInputManager()
{

}

CInputManager::~CInputManager()
{
}

void CInputManager::InputDetected(std::string strShortDeviceName, unsigned char cRecievedKey)
{
    // TODO: Handling and registration building
    std::string cFilteredKey;
    std::string strRegistrationPlate;

    if (isdigit(cRecievedKey) || isalpha(cRecievedKey))             // Check for valid alphanumeric key
    {
        cFilteredKey = std::toupper(cRecievedKey);                   // Convert to Upper case to avoid mixing cases
        strRegistrationPlate = pRegistration.BuildRegistration(cFilteredKey);
    }
    else if (cRecievedKey == pDevices.m_cReturnKeyVirtualCode)       // Check if Return Key/End of string key has been entered
    {
        cFilteredKey = pDevices.m_strReturnKeyMessage;                // Mark with '!'
        strRegistrationPlate = pRegistration.BuildRegistration(cFilteredKey);

        #ifndef DEBUG
        OutputDebugString((LPCSTR)strRegistrationPlate.c_str());   // Debug output built registration 
        OutputDebugString(" ");
        #endif
    }

    #ifndef DEBUG
    OutputDebugString((LPCSTR)strShortDeviceName.c_str());   // Debug output device name
    OutputDebugString(" ");

    OutputDebugString((LPCSTR)cFilteredKey.c_str());   // Debug output entered key
    OutputDebugString("\n");
    #endif
}

void CInputManager::CheckVehicle()
{

}

void CInputManager::AddVehicle()
{

}

void CInputManager::RemoveVehicle()
{

}
