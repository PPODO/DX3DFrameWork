#include "ActorClass.h"
#include "Actor.h"
#include "PlayerClass.h"
#include "DefaultEnemy.h"
#include "StageClass.h"
#include "MenuStage.h"
#include "InGameStage.h"

ActorClass::ActorClass() : m_CurrentStage(0) {
}

ActorClass::~ActorClass() {
	for (Actor* AActor : m_Stages) {
		if (AActor) {
			AActor->Destroy();
		}
	}
	m_Stages.clear();

	if (m_PoolManager) {
		delete m_PoolManager;
		m_PoolManager = nullptr;
	}
}

bool ActorClass::Init(LPDIRECT3DDEVICE9 Device) {
	m_PoolManager = new ObjectPoolClass(Device);
	if (!m_PoolManager) { return false; }
	m_PoolManager->CreateObject<DefaultEnemy>("DefaultEnemy", 10);

	new MenuStage(m_Stages);
	new InGameStage(m_Stages, m_PoolManager);

	m_Player = new PlayerClass;
	if (!m_Player) {
		return false;
	}

	m_Player->Init(Device);
	for (Actor* AActor : m_Stages) {
		if (AActor) {
			AActor->Init(Device);
		}
	}
	return true;
}

void ActorClass::Frame(float DeltaTime) {
	if (m_Stages[m_CurrentStage]) {
		m_Stages[m_CurrentStage]->Update(DeltaTime);
	}
	if (m_CurrentStage > 0) {
		m_Player->Update(DeltaTime);
	}
}

void ActorClass::Render(LPD3DXSPRITE Sprite) {
	if (m_Stages[m_CurrentStage]) {
		m_Stages[m_CurrentStage]->Render(Sprite);
	}
	if (m_CurrentStage > 0) {
		m_Player->Render(Sprite);
	}
}