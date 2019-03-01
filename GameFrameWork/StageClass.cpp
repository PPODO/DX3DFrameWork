#include "StageClass.h"
#include "SystemClass.h"
#include "BackGroundUIClass.h"
#include "Actor.h"
#include "ActorClass.h"

StageClass::StageClass(std::vector<StageClass*>& Stages) : m_bUseTimer(false), m_StageTime(0.f), m_bIsStop(false), m_bNotificationForStop(false) {
	Stages.push_back(this);
}

StageClass::~StageClass() {
}

void StageClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);
	if (m_bUseTimer && m_StageTime < std::chrono::system_clock::now() - m_StartTime) {
		if (m_bIsStop) {
			ReleaseForChangingStage();
			SystemClass::GetInst()->GetActorManager()->SetCurrentStage(SystemClass::GetInst()->GetActorManager()->GetCurrentStage() + 1);
		}
		m_bNotificationForStop = true;
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