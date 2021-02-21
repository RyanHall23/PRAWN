#include "Registration.h"


CRegistration::CRegistration()
{

}

CRegistration::~CRegistration()
{

}

/// <summary>
/// Build registration using registration string member
/// </summary>
/// <param name="translatedKey"></param>
/// <returns></returns>
std::string CRegistration::BuildRegistration(char translatedKey, int iDeviceIndex, int iDeviceTotal)
{
	m_vecStrRegistration.resize(iDeviceTotal);

	if (IsAlphaNumeric(translatedKey))
	{
		switch (translatedKey)
		{
		case m_cNewLine:						// Is a new line key				(\n)
		case m_cCarriageReturn:					// Is a carraige return key			(\r)
			if (m_vecStrRegistration.at(iDeviceIndex).length() > 0)	// If not empty (Something can be returned)
			{
				return ReturnInputEvent(iDeviceIndex);
			}
			break;
		default:								// Is alphanumeric					(A-9)
			m_vecStrRegistration.at(iDeviceIndex) += std::toupper(translatedKey);		// Add to string, converting all to upper case
			break;
		}
	}

	return NULLSTRING;
}

/// <summary>
/// Check if key is a valid AlphaNumeric Key, if true (Including \n && \r) return true, else special keys (,.;'[]~: etc) return false
/// </summary>
/// <param name="translatedKey"></param>
/// <returns></returns>
bool CRegistration::IsAlphaNumeric(char translatedKey)
{
	return (translatedKey >= 0 && translatedKey <= 255);
}

/// <summary>
/// Clear string after return key event
/// </summary>
/// <returns></returns>
std::string CRegistration::ReturnInputEvent(int iDeviceIndex)
{
	std::string strReturnRegistration = m_vecStrRegistration.at(iDeviceIndex);
	m_vecStrRegistration.at(iDeviceIndex) = NULLSTRING;
	return strReturnRegistration;
}