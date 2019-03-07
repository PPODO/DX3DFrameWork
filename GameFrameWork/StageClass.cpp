#include "StageClass.h"
#include "SystemClass.h"
#include "BackGroundClass.h"
#include "ActorClass.h"
#include "Actor.h"

StageClass::StageClass(std::vector<StageClass*>& Stages) : m_bUseTimer(false), m_StageTime(0.f), m_bIsStop(false) {
	Stages.push_back(this);
}

StageClass::~StageClass() {
}

bool StageClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {

	return true;
}

void StageClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	if (m_bUseTimer && m_StageTime < std::chrono::system_clock::now() - m_StartTime) {
		
	}
	m_BackGround->Update(DeltaTime);
}

void StageClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

	m_BackGround->Render(Sprite);
}

void StageClass::Destroy() {
	if (m_BackGround) {
		m_BackGround->Destroy();
		delete m_BackGround;
		m_BackGround = nullptr;
	}
}

void StageClass::ChangeStageNotification() {
}

void StageClass::ReleaseForChangingStage() {
}