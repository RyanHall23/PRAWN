#include "Registration.h"


CRegistration::CRegistration()
{

}

CRegistration::~CRegistration()
{

}

std::string CRegistration::BuildRegistration(std::string translatedKey)
{
	if (translatedKey != m_strReturnKeyMessage)	// If key is part of the registration plate and not end of line marker
	{
		m_vecstrBuildingReg.push_back(translatedKey);	// Add character to vector to build registration
	}

	else if (translatedKey == m_strReturnKeyMessage)		// If end string marker
	{
		if (m_vecstrBuildingReg.capacity() > 0) // If licence plate is populated before return is hit
		{
			for (unsigned int i = 0; i < m_vecstrBuildingReg.size(); i++)	// Increment through size of licence plate
			{
				m_ssRegBuilder << m_vecstrBuildingReg.at(i);	// Build as a string
			}

			m_strTempReg = m_ssRegBuilder.str();		// Convert to temp string
			m_strCompletedRegPlate = m_strTempReg;		// Assign completed registration

			m_strTempReg = NULLSTRING;					// Clear temp string
			m_ssRegBuilder.str(NULLSTRING);				// Clear strigstream
			m_vecstrBuildingReg.clear();				// Clear vector

			return m_strCompletedRegPlate;	// Return licence plate registration
		}
	}
	return NULLSTRING; // Return null value if licence plate isn't built fully
}