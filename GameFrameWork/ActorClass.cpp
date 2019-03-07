#include "ActorClass.h"
#include "PlayerClass.h"
#include "StageClass.h"
#include "MenuStage.h"
#include "IntroduceStage.h"
#include "PlayMethodStage.h"
#include "ScoreStage.h"
#include "CreditStage.h"
#include "InGameStage.h"
#include "DefaultEnemyClass.h"
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

	if (m_Player) {
		m_Player->Destroy();
		delete m_Player;
		m_Player = nullptr;
	}

	if (m_PoolManager) {
		delete m_PoolManager;
		m_PoolManager = nullptr;
	}
}

bool ActorClass::Init(LPDIRECT3DDEVICE9 Device) {
	m_PoolManager = new ObjectPoolClass(Device);
	if (!m_PoolManager) { return false; }
	m_PoolManager->CreateObject<DefaultEnemyClass>("DefaultEnemy", 10, ES_DEFAULT);
	m_PoolManager->CreateObject<DefaultProjectileClass>("DefaultProjectile", 150, PS_DEFAULT);

	m_Player = new PlayerClass(m_PoolManager);
	if (!m_Player) {
		return false;
	}
	m_Player->Init(Device);

	new MenuStage(m_Stages);
	new IntroduceStage(m_Stages);
	new PlayMethodStage(m_Stages);
	new ScoreStage(m_Stages);
	new CreditStage(m_Stages);
	(new InGameStage(m_Stages, 5))->SetPlayerAndPoolManager(m_Player, m_PoolManager);

	for (auto Stages : m_Stages) {
		if (Stages) {
			Stages->Init(Device);
		}
	}
	return true;
}

void ActorClass::Frame(float DeltaTime) {
	if (m_CurrentStage < m_Stages.size()) {
		m_Stages[m_CurrentStage]->Update(DeltaTime);
	}
}

void ActorClass::Render(LPD3DXSPRITE Sprite) {
	if (m_CurrentStage < m_Stages.size()) {
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