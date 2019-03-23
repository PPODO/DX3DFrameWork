#include "InGameStage.h"
#include "PlayerClass.h"
#include "EnemyClass.h"
#include "ObstacleClass.h"
#include "ObjectPoolClass.h"
#include "BackGroundClass.h"
#include "SystemClass.h"
#include "EventClass.h"

InGameStage::InGameStage(ObjectPoolClass* OP, class PlayerClass* Player) : m_PoolManager(OP), m_Player(Player) {
	m_RandomAlgorithm = std::mt19937_64(m_RandomDevice());
}

InGameStage::~InGameStage() {
	ReleasePoolObjects();

	for (const auto& Iterator : m_BackGroundImages) {
		if (Iterator.first) {
			delete Iterator.first;
		}
		if (Iterator.second) {
			delete Iterator.second;
		}
	}
	m_BackGroundImages.clear();
	m_PoolManager = nullptr;
	m_Player = nullptr;
}

bool InGameStage::Init(LPDIRECT3DDEVICE9 Device) {
	
	return true;
}

void InGameStage::Update(float DeltaTime) {
	StageClass::Update(DeltaTime);

	if (m_Player) {
		m_Player->Update(DeltaTime);
		
		if (!m_Player->GetStartMoveToLocation()) {
			UpdatePoolObjects(DeltaTime);
		}

		for (const auto& Iterator : m_BackGroundImages) {
			if (Iterator.first->GetActorImage() && Iterator.second->GetActorImage()) {
				Iterator.first->SetAdditionalSpeed(D3DXVECTOR2(m_Player->GetIsMoving(), 0.f));
				Iterator.second->SetAdditionalSpeed(D3DXVECTOR2(m_Player->GetIsMoving(), 0.f));
				Iterator.first->GetActorImage()->AddYPosition(m_Player->GetCurrentHeight() * -1);
				Iterator.second->GetActorImage()->AddYPosition(m_Player->GetCurrentHeight() * -1);
			}
			Iterator.first->Update(DeltaTime);
			Iterator.second->Update(DeltaTime);
		}

		UpdatePoolObjects(DeltaTime);
	}
	PickUpObject();
}

void InGameStage::Render(LPD3DXSPRITE Sprite) {
	StageClass::Render(Sprite);

	for (const auto& Iterator : m_BackGroundImages) {
		Iterator.first->Render(Sprite);
		Iterator.second->Render(Sprite);
	}

	if (m_Player) {
		m_Player->Render(Sprite);
	}

	RenderActivatedPoolObjects(Sprite);
}

void InGameStage::ChangeStageNotification() {
	if (m_PoolManager) {
	}
}

void InGameStage::ReleaseStageNotification() {
	if (m_PoolManager) {
		ReleasePoolObjects();
	}
}

void InGameStage::UpdatePoolObjects(const float& DeltaTime) {
	
}

void InGameStage::RenderActivatedPoolObjects(const LPD3DXSPRITE& Sprite) {

}

void InGameStage::ReleasePoolObjects() {
	
}