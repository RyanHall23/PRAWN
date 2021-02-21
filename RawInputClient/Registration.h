#pragma once
#include <string>
#include <vector>

class CRegistration
{
private:
	std::vector<std::string>	m_vecStrRegistration;

	const std::string			NULLSTRING = "";
	static constexpr char		m_cNewLine = '\n';
	static constexpr char		m_cCarriageReturn = '\r';

	int							m_iNumOfDevies = 0;

public:
	CRegistration();
	~CRegistration();

	std::string BuildRegistration(char translatedKey, int iDeviceIndex, int iDeviceTotal);
	bool IsAlphaNumeric(char translatedKey);
	std::string ReturnInputEvent(int iDeviceIndex);


};