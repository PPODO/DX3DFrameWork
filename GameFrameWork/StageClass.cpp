#include "StageClass.h"
#include "Actor.h"
#include "BackGroundUIClass.h"

StageClass::StageClass(std::vector<Actor*>& Stages) : m_bUseTimer(false), m_StageTime(0.f) {
	Stages.push_back(this);
}

StageClass::~StageClass() {
}

bool StageClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	

	return true;
}

void StageClass::Update(float DeltaTime) {
	if (m_bUseTimer && m_StageTime < std::chrono::system_clock::now() - m_StartTime) {
		
	}
}