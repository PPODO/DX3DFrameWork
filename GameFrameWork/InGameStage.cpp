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

	m_PoolObjects.resize(EOT_COUNT); 
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
		m_Player->Update(DeltaTime, 0.f);
		
		if (!m_Player->GetStartMoveToLocation()) {

		}

		for (const auto& Iterator : m_BackGroundImages) {
			if (Iterator.first->GetActorImage() && Iterator.second->GetActorImage()) {
				Iterator.first->SetAdditionalSpeed(D3DXVECTOR2(m_Player->GetIsMoving(), 0.f));
				Iterator.second->SetAdditionalSpeed(D3DXVECTOR2(m_Player->GetIsMoving(), 0.f));
			}
			Iterator.first->Update(DeltaTime, m_Player->GetCurrentHeight() * -1);
			Iterator.second->Update(DeltaTime, m_Player->GetCurrentHeight() * -1);
		}
		UpdatePoolObjects(DeltaTime, m_Player->GetCurrentHeight() * -1);
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

void InGameStage::UpdatePoolObjects(const float& DeltaTime, float ActorHeight) {
	for (size_t i = 0; i < m_ActivatedObjects.size(); i++) {
		for (auto Iterator = m_ActivatedObjects[i].begin(); Iterator != m_ActivatedObjects[i].end();) {
			if ((*Iterator).second && (*Iterator).second->GetActorIsActivated()) {
				(*Iterator).second->Update(DeltaTime, ActorHeight);
				++Iterator;
			}
			else {
				(*Iterator).second->ClearObject();
				m_PoolObjects[i][(*Iterator).first].push((*Iterator).second);
				Iterator = m_ActivatedObjects[i].erase(Iterator);
			}
		}
	}
}

void InGameStage::RenderActivatedPoolObjects(const LPD3DXSPRITE& Sprite) {
	for (auto& It : m_ActivatedObjects) {
		for (auto& Iterator : It) {
			if (Iterator.second && Iterator.second->GetActorIsActivated()) {
				Iterator.second->Render(Sprite);
			}
		}
	}
}

void InGameStage::ReleasePoolObjects() {
	for (size_t i = 0; i < m_ActivatedObjects.size(); i++) {
		for (size_t j = 0; j < m_ActivatedObjects[i].size(); j++) {
			m_PoolObjects[i][m_ActivatedObjects[i][j].first].push(m_ActivatedObjects[i][j].second);
		}
		m_ActivatedObjects[i].clear();
	}
	m_ActivatedObjects.clear();

	for (auto& It : m_PoolObjects) {
		for (auto& Iterator : It) {
			if (Iterator.size() > 0) {
				m_PoolManager->ReleaseAll(Iterator.top()->GetName(), Iterator, Iterator.size());
			}
		}
	}
}