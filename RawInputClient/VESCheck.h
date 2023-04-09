#pragma once

#include "curl/curl.h"
#include <string>
#include <iostream>

class CVESCheck
{
public:
	CVESCheck();
	~CVESCheck();

	int CheckTaxMOT(std::string str_Reg);
private:
	std::string m_strAPIKey = "x-api-key: KEY_HERE";
	std::string m_strContent = "Content - Type: application / json";
};