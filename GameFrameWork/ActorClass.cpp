#include "ActorClass.h"
#include "MainStage.h"
#include "FirstInGameStage.h"
#include "PlayerClass.h"
#include "DefaultEnemy.h"
#include "HoleClass.h"
#include "DefaultProjectile.h"

ActorClass::ActorClass() : m_CurrentStage(0), m_PlayerClass(nullptr) {
}

ActorClass::~ActorClass() {
	for (auto& It : m_Stages) {
		if (It) {
			delete It;
		}
	}
	m_Stages.clear();

	if (m_PlayerClass) {
		delete m_PlayerClass;
		m_PlayerClass = nullptr;
	}
	if (m_PoolManager) {
		delete m_PoolManager;
		m_PoolManager = nullptr;
	}
}

bool ActorClass::Init(LPDIRECT3DDEVICE9 Device) {
	m_PoolManager = new ObjectPoolClass(Device);
	if (!m_PoolManager) { return false; }

	m_PoolManager->CreateObject<HoleClass>("Obstacle_Hole", 20, EOT_HOLE);
	m_PoolManager->CreateObject<DefaultEnemy>("Enemy_Default", 10, EET_DEFAULT);
	m_PoolManager->CreateObject<DefaultProjectile>("Projectile_Default", 200, EPT_DEFAULT);

	m_PlayerClass = new PlayerClass(m_PoolManager);
	if (!m_PlayerClass || !m_PlayerClass->Init(Device)) {
		return false;
	}

	m_Stages.push_back(new MainStage);
	m_Stages.push_back(new FirstInGameStage(m_PoolManager, m_PlayerClass));
	for (const auto& Iterator : m_Stages) {
		if (Iterator) {
			Iterator->Init(Device);
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
		m_Stages[m_CurrentStage]->ReleaseStageNotification();
		m_CurrentStage = Value;
		m_Stages[m_CurrentStage]->ChangeStageNotification();
	}
}