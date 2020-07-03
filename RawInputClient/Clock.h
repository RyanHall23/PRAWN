#pragma once

#include <cstdio>
#include <ctime>
#include <chrono>

class CClock			//Class to create a clock for tracking distance/speed
{
public:
	CClock();			// Constructor
	~CClock();			// Destructor
	double GetTime();	// GetTime Declaration
private:
	std::clock_t	m_clStartTime;
	double			m_dbDuration;
};
