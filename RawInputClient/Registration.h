#pragma once
#include <string>
#include <vector>

class CRegistration
{
private:
	std::string					m_strRegistration;

	const std::string			NULLSTRING = "";
	static constexpr char		m_cNewLine = '\n';
	static constexpr char		m_cCarriageReturn = '\r';

	int							m_iNumOfDevies = 0;
	std::vector<std::vector<std::string>> a;

public:
	CRegistration();
	~CRegistration();

	std::string BuildRegistration(char translatedKey, int deviceIndex);
	bool IsAlphaNumeric(char translatedKey);
	std::string ReturnInputEvent();


};