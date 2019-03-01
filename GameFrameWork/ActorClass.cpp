#include "ActorClass.h"
#include "StageClass.h"
#include "MenuStage.h"
#include "InGameStage.h"
#include "DefaultEnemy.h"
#include "RushEnemy.h"
#include "SplitEnemy.h"
#include "FlightEnemy.h"
#include "PlayerClass.h"
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
	m_PoolManager->CreateObject<DefaultEnemy>("DefaultEnemy", 10, EN_DEFAULT);
	m_PoolManager->CreateObject<RushEnemy>("RushEnemy", 10, EN_RUSH);
	m_PoolManager->CreateObject<SplitEnemy>("SplitEnemy", 10, EN_SPLIT);
	m_PoolManager->CreateObject<FlightEnemy>("FlightEnemy", 10, EN_FLIGHT);
//	m_PoolManager->CreateObject<DefaultProjectileClass>("DefaultProjectile", 100);

	m_Player = new PlayerClass(m_PoolManager);
	if (!m_Player) {
		return false;
	}
	m_Player->Init(Device);

	new MenuStage(m_Stages);
	new InGameStage(m_Stages, 5);
	(new InGameStage(m_Stages, 10))->SetObjectPoolAndPlayerClass(m_PoolManager, m_Player);

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
		m_CurrentStage = Value;
		m_Stages[m_CurrentStage]->ChangeStageNotification();
	}
}