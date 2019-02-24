#include "ActorClass.h"
#include "Actor.h"
#include "PlayerClass.h"
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
	m_PoolManager = new ObjectPoolClass;

	new MenuStage(m_Stages);
	new InGameStage(m_Stages);

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
}