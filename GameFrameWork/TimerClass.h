#pragma once
#pragma comment(lib, "winmm.lib")

#include <Windows.h>

class TimerClass {
private:
	DWORD m_FrameCount, m_LastTime;
	float m_TimeElapsed;

public:
	TimerClass();
	~TimerClass();

public:
	bool UpdateFPS(float& DeltaTime);
};

