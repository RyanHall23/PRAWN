#pragma once

#include <string>
#include <vector>

class CVehicle
{
public:
	CVehicle();
	~CVehicle();

	std::string m_strRegistration;
	int			m_iDirectionOrigin;
	double		m_dbTotalTravelTime;

	// Must match amount of scanning devices and match indexes (E.G. Dev 1 @ Index 0 == 1.91 | Dev 2 @ Index 1 == 3.45 | Dev 3  @ Index 2 == NULL | Dev 4 @ Index 3 == 17.29)
	std::vector<double>m_vecDbScanningTimes;

};