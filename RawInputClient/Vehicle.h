#pragma once

#include <string>

class CVehicle
{
public:
	CVehicle();
	~CVehicle();

	std::string m_strRegistration;
	int			m_iDirectionOrigin;
	double		m_dbStartTime;
	double		m_dbEndTime;
	double		m_dbTotalTravelTime;
};

