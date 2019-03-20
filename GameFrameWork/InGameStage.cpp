#include "InGameStage.h"
#include "PlayerClass.h"
#include "ObjectPoolClass.h"
#include "BackGroundClass.h"
#include "SystemClass.h"
#include "EventClass.h"
#include <iostream>

InGameStage::InGameStage(ObjectPoolClass* OP, class PlayerClass* Player) : m_PoolManager(OP), m_Player(Player) {
	m_FileSrc.push_back(std::make_pair(L"Stage/BackGround1.png", L"Stage/BackGround1.png"));
	m_FileSrc.push_back(std::make_pair(L"Stage/BackGround2.png", L"Stage/BackGround2.png"));
	m_FileSrc.push_back(std::make_pair(L"Stage/BackGround3.png", L"Stage/BackGround3.png"));
	m_FileSrc.push_back(std::make_pair(L"Stage/Ground.png", L"Stage/Ground.png"));

	std::vector<std::pair<std::string, size_t>> ObstacleName;
	ObstacleName.push_back(std::make_pair("Obstacle_Hole", 10));



	m_PoolObjectType.insert(std::make_pair("Obstacle", ObstacleName));
}

InGameStage::~InGameStage() {
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
	RECT WindowSize = SystemClass::GetInst()->GetWindowSize();
	D3DXVECTOR3 FirstImageLocation(0.f, 0.f, 0.f), SecondImageLocation(0.f, 0.f, 0.f);

	for (size_t i = 0; i < m_FileSrc.size(); i++) {
		std::pair<BackGroundClass*, BackGroundClass*> BackGroundImage(new BackGroundClass, new BackGroundClass);
		if ((!BackGroundImage.first || !BackGroundImage.first->Init(Device, m_FileSrc[i].first)) || (!BackGroundImage.second || !BackGroundImage.second->Init(Device, m_FileSrc[i].second))) {
			return false;
		}
		switch (i) {
		case EBGT_BACKGROUND2:
		case EBGT_BACKGROUND3:
			BackGroundImage.first->SetActorMoveSpeed(m_BackGroundImages[i - 1].first->GetActorMoveSpeed() / 2);
			BackGroundImage.second->SetActorMoveSpeed(m_BackGroundImages[i - 1].second->GetActorMoveSpeed() / 2);
		case EBGT_BACKGROUND:
			FirstImageLocation = BackGroundImage.first->GetActorImage()->GetImageCenter();
			SecondImageLocation = D3DXVECTOR3(FLOAT(BackGroundImage.second->GetActorImage()->GetRect().right), 0.f, 0.f) + FirstImageLocation;
			break;
		case EBGT_GROUND:
			BackGroundImage.first->SetActorCollisionType(ECT_ALLBLOCK);
			BackGroundImage.second->SetActorCollisionType(ECT_ALLBLOCK);
			EventClass::GetInst()->BindCollisionEvent(BackGroundImage.first);
			EventClass::GetInst()->BindCollisionEvent(BackGroundImage.second);
			FirstImageLocation = D3DXVECTOR3(BackGroundImage.first->GetActorImage()->GetImageCenter().x, WindowSize.bottom - BackGroundImage.first->GetActorImage()->GetImageCenter().y, 0.f);
			SecondImageLocation = D3DXVECTOR3(BackGroundImage.second->GetActorImage()->GetRect().right + FirstImageLocation.x, FirstImageLocation.y, 0.f);
			break;
		default:
			throw;
		}
		BackGroundImage.first->GetActorImage()->SetPosition(FirstImageLocation);
		BackGroundImage.second->GetActorImage()->SetPosition(SecondImageLocation);
		m_BackGroundImages.push_back(BackGroundImage);
	}


	///////////////
	if (m_Player && EBGT_GROUND < m_BackGroundImages.size()) {
		D3DXVECTOR3 Location = m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetPosition() - m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetImageCenter();
		m_Player->GetActorImage()->SetPosition(D3DXVECTOR3(0.f, Location.y, 0.f) - m_Player->GetActorImage()->GetImageCenter());
	}
	EventClass::GetInst()->WakeUpEventThread();

	if (m_PoolManager) {
		for (auto It : m_PoolObjectType) {
			std::vector<std::stack<Actor*>> Objects;
			for (int i = 0; i < m_PoolManager->GetObjectTypeCountByName(It.second[(i < It.second.size() ? i : i - 1)].first); i++) {
				std::stack<Actor*> Object;
				m_PoolManager->GetPoolObject(It.second[i].first, Object, It.second[i].second);
				Objects.push_back(Object);
			}
			m_PoolObjects.push_back(Objects);
		}
	}

	///////////////
	
	return true;
}

void InGameStage::Update(float DeltaTime) {
	StageClass::Update(DeltaTime);

	for (const auto& Iterator : m_BackGroundImages) {
		Iterator.first->Update(DeltaTime);
		Iterator.second->Update(DeltaTime);
	}

	if (m_Player) {
		m_Player->Update(DeltaTime);
	}

	UpdatePoolObjects();
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
}

void InGameStage::ChangeStageNotification() {
	if (m_PoolManager) {

	}
}

void InGameStage::ReleaseStageNotification() {
	if (m_PoolManager) {

	}
}

void InGameStage::UpdatePoolObjects() {


}