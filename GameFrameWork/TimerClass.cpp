#include "TimerClass.h"
#include <ctime>

TimerClass::TimerClass() : m_FrameCount(0), m_LastTime(0), m_TimeElapsed(0) {
}

TimerClass::~TimerClass() {
}

bool TimerClass::UpdateFPS(float& DeltaTime) {
	// 초당 FPS 제한, DeltaTime 구하는 공식
	DWORD CurTime = timeGetTime();
	DeltaTime = (CurTime - m_LastTime) * 0.001f;
	m_TimeElapsed += DeltaTime;
	m_FrameCount++;

	if (m_TimeElapsed >= 1 / 60) {
		m_FrameCount = 0;
		m_TimeElapsed = 0;
		m_LastTime = CurTime;
		return true;
	}
	m_LastTime = CurTime;
	return false;
}