#pragma once
#include <vector>
#include <string>
#include <sstream>

constexpr auto NULLSTRING = "";		// Set string to null for clearance to reset it


class CRegistration
{
public:
	CRegistration();	// Constructor
	~CRegistration();	// Destructor

	std::string BuildRegistration(std::string translatedKey);	// Function for combining "Key presses" into complete licence plate reg from raw input
private:

	std::vector<std::string>	m_vecstrBuildingReg;					// Vector for building characters of registration plate
	std::string					m_strTempReg;						// Temporary string for storage to help with clearance
	std::string					m_strCompletedRegPlate;				// Completed licence plate string
	std::stringstream			m_ssRegBuilder;						// Stringstream to combine vector indexes together into a string
	const std::string			m_strReturnKeyMessage = "!";		// Marks the end of the string with "return" notifier
};