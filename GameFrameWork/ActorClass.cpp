#include "ActorClass.h"
#include "StageClass.h"
#include "MenuStage.h"
#include "IntroduceStage.h"
#include "PlayMethodStage.h"
#include "ScoreStage.h"
#include "CreditStage.h"
#include "DefaultProjectileClass.h"

ActorClass::ActorClass() : m_CurrentStage(0) {
}

ActorClass::~ActorClass() {
	for (auto Stages : m_Stages) {
		if (Stages) {
			Stages->Destroy();
			delete Stages;
			Stages = nullptr;
		}
	}
	m_Stages.clear();

	if (m_PoolManager) {
		delete m_PoolManager;
		m_PoolManager = nullptr;
	}
}

bool ActorClass::Init(LPDIRECT3DDEVICE9 Device) {
	// 적이나 총알같은 경우는 미리 여러개의 객체를 만들어 놓고 필요할 때만 사용하는
	// 풀 패턴을 쓰는게 효과적임
	m_PoolManager = new ObjectPoolClass(Device);
	if (!m_PoolManager) { return false; }

	new MenuStage(m_Stages);
	new IntroduceStage(m_Stages);
	new PlayMethodStage(m_Stages);
	new ScoreStage(m_Stages);
	new CreditStage(m_Stages);

	for (auto Stages : m_Stages) {
		if (Stages) {
			Stages->Init(Device);
		}
	}
	return true;
}

void ActorClass::Frame(float DeltaTime) {
	if (m_Stages[m_CurrentStage]) {
		m_Stages[m_CurrentStage]->Update(DeltaTime);
	}
}

void ActorClass::Render(LPD3DXSPRITE Sprite) {
	if (m_Stages[m_CurrentStage]) {
		m_Stages[m_CurrentStage]->Render(Sprite);
	}
}

void ActorClass::SetCurrentStage(unsigned short Value) {
	if (Value < m_Stages.size()) {
		unsigned short BeforeStage = m_CurrentStage;
		m_CurrentStage = Value;
		m_Stages[BeforeStage]->ReleaseForChangingStage();
		m_Stages[m_CurrentStage]->ChangeStageNotification();
	}
}