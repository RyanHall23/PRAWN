#include "Clock.h"

CClock::CClock()
{
	m_clStartTime = 0;
	m_dbDuration = 0;
}

CClock::~CClock()
{
}

double CClock::GetTime()
{
	m_dbDuration = (std::clock() - m_clStartTime) / (double)CLOCKS_PER_SEC;	//Calculate current time
	return m_dbDuration;
}
