#pragma once
#include <string>

class CRegistration
{
private:
	std::string					m_strRegistration;

	const std::string			NULLSTRING = "";

	static constexpr char		m_cNewLine = '\n';
	static constexpr char		m_cCarriageReturn = '\r';

public:
	CRegistration();
	~CRegistration();

	std::string BuildRegistration(char translatedKey);
	bool IsAlphaNumeric(char translatedKey);
	std::string ReturnInputEvent();
};