#include "TimerClass.h"
#include <ctime>
#include <iostream>

TimerClass::TimerClass() : m_FrameCount(0), m_LastTime(0), m_TimeElapsed(0) {
}

TimerClass::~TimerClass() {
}

bool TimerClass::UpdateFPS(float& DeltaTime) {
	DWORD CurTime = timeGetTime();
	DeltaTime = float(CurTime - m_LastTime) * 0.001f;
	m_TimeElapsed += DeltaTime;
	m_FrameCount++;

	if (m_TimeElapsed >= 1 / 60) {
		std::cout << m_TimeElapsed << std::endl;
		m_FrameCount = 0;
		m_TimeElapsed = 0;
		m_LastTime = CurTime;
		return true;
	}
	m_LastTime = CurTime;
	return false;
}