#include "FirstInGameStage.h"
#include "EnemyClass.h"
#include "ObstacleClass.h"
#include "ObjectPoolClass.h"
#include "BackGroundClass.h"
#include "PlayerClass.h"
#include "SystemClass.h"
#include "EventClass.h"

FirstInGameStage::FirstInGameStage(ObjectPoolClass * OP, PlayerClass * Player) : InGameStage(OP, Player) {
	m_FileSrc.push_back(std::make_pair(L"Stage/BackGround1.png", L"Stage/BackGround1.png"));
	m_FileSrc.push_back(std::make_pair(L"Stage/BackGround2.png", L"Stage/BackGround2.png"));
	m_FileSrc.push_back(std::make_pair(L"Stage/Ground.png", L"Stage/Ground.png"));

	m_ActivatedObjects.resize(EOT_COUNT);
	m_PoolObjects[EOT_ENEMY].resize(EET_COUNT);
	m_PoolObjects[EOT_OBSTACLE].resize(EObstacleType::EOT_COUNT);

	m_ObjectMaxSpawnCount.push_back(10);
	m_ObjectMaxSpawnCount.push_back(10);
	m_ObjectSpawnDelayTime.push_back(std::make_tuple(std::chrono::system_clock::now(), std::chrono::duration<float>(0.f), 5.f));
	m_ObjectSpawnDelayTime.push_back(std::make_tuple(std::chrono::system_clock::now(), std::chrono::duration<float>(0.f), 7.f));
}

FirstInGameStage::~FirstInGameStage() {
}

bool FirstInGameStage::Init(LPDIRECT3DDEVICE9 Device) {
	InGameStage::Init(Device);

	for (size_t i = 0; i < m_FileSrc.size(); i++) {
		D3DXVECTOR3 FirstLocation, SecondLocation;
		std::pair<BackGroundClass*, BackGroundClass*> BackGroundImage(new BackGroundClass, new BackGroundClass);
		if (!BackGroundImage.first || !BackGroundImage.first->Init(Device, m_FileSrc[i].first) || !BackGroundImage.second || !BackGroundImage.second->Init(Device, m_FileSrc[i].second)) {
			return false;
		}
		switch (i) {
		case EBGT_BACKGROUND:
		case EBGT_BACKGROUND2:
			FirstLocation = D3DXVECTOR3(0.f, BackGroundImage.first->GetActorImage()->GetImageCenter().y - 100, 0.f);
			SecondLocation = D3DXVECTOR3(FLOAT(BackGroundImage.second->GetActorImage()->GetRect().right), 0.f, 0.f) + FirstLocation;
			break;
		case EBGT_GROUND:
			BackGroundImage.first->SetActorCollisionType(ECT_ALLBLOCK);
			BackGroundImage.second->SetActorCollisionType(ECT_ALLBLOCK);
			EventClass::GetInst()->BindCollisionEvent(BackGroundImage.first);
			EventClass::GetInst()->BindCollisionEvent(BackGroundImage.second);
			FirstLocation = D3DXVECTOR3(BackGroundImage.first->GetActorImage()->GetImageCenter().x, GetWindowSize().bottom - BackGroundImage.first->GetActorImage()->GetImageCenter().y, 0.f);
			SecondLocation = D3DXVECTOR3(FLOAT(BackGroundImage.second->GetActorImage()->GetRect().right), 0.f, 0.f) + FirstLocation;
			break;
		}
		BackGroundImage.first->GetActorImage()->SetPosition(FirstLocation);
		BackGroundImage.second->GetActorImage()->SetPosition(SecondLocation);
		BackGroundImage.first->SetOtherBackGroundActor(BackGroundImage.second);
		BackGroundImage.second->SetOtherBackGroundActor(BackGroundImage.first);
		m_BackGroundImages.push_back(BackGroundImage);
	}
	m_EnemySpawnLocation = D3DXVECTOR3(FLOAT(GetWindowSize().right), FLOAT(GetWindowSize().bottom / 2) / 2, 0.f);
	m_ObstacleSpawnLocation = D3DXVECTOR3(m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetPosition() - m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetImageCenter());

	return true;
}

void FirstInGameStage::Update(float DeltaTime) {
	InGameStage::Update(DeltaTime);
}

void FirstInGameStage::Render(LPD3DXSPRITE Sprite) {
	InGameStage::Render(Sprite);

}

void FirstInGameStage::ChangeStageNotification() {
	InGameStage::ChangeStageNotification();

	if (GetPoolClass()) {
		GetPoolClass()->GetPoolObject("Enemy_Default", m_PoolObjects[EOT_ENEMY][EET_DEFAULT], m_ObjectMaxSpawnCount[EOT_ENEMY]);
		GetPoolClass()->GetPoolObject("Obstacle_Hole", m_PoolObjects[EOT_OBSTACLE][EOT_HOLE], m_ObjectMaxSpawnCount[EOT_OBSTACLE]);
	}

	if (GetPlayerClass()) {
		D3DXVECTOR3 Location = m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetPosition() - m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetImageCenter();
		GetPlayerClass()->GetActorImage()->SetPosition(D3DXVECTOR3(0.f, Location.y, 0.f) - GetPlayerClass()->GetActorImage()->GetImageCenter());
	}
	EventClass::GetInst()->WakeUpEventThread();
}

void FirstInGameStage::ReleaseStageNotification() {
	InGameStage::ReleaseStageNotification();

}

void FirstInGameStage::PickUpObject() {
	for (size_t i = 0; i < m_ActivatedObjects.size(); i++) {
		if (GetPlayerClass() && m_ActivatedObjects[i].size() < m_ObjectMaxSpawnCount[i]) {
			if (std::chrono::system_clock::now() - std::get<0>(m_ObjectSpawnDelayTime[i]) > std::get<1>(m_ObjectSpawnDelayTime[i])) {
				size_t Index = std::uniform_int_distribution<size_t>(0, m_PoolObjects[i].size() - 1)(m_RandomAlgorithm);
				auto Object = m_PoolObjects[i][Index].top();
				if (Object) {
					float Height = 0;
					switch (i) {
					case EOT_OBSTACLE:
						Height = (m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetPosition().y - m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetImageCenter().y) - m_ObstacleSpawnLocation.y;
						Object->SpawnActor(m_ObstacleSpawnLocation + D3DXVECTOR3(0.f, Height, 0.f));
						break;
					case EOT_ENEMY:
						auto EnemyObject = (EnemyClass*)Object;
						if (EnemyObject) {
							Height = m_BackGroundImages[EBGT_BACKGROUND].first->GetActorImage()->GetPosition().y - m_EnemySpawnLocation.y;
							EnemyObject->SetPoolManager(GetPoolClass());
							EnemyObject->SpawnActor(D3DXVECTOR3(FLOAT(GetWindowSize().right - Object->GetActorImage()->GetImageCenter().x), Height, 0.f));
						}
						break;
					}
					m_ActivatedObjects[i].push_back(std::make_pair(Index, Object));
					m_PoolObjects[i][Index].pop();
				}
				std::get<1>(m_ObjectSpawnDelayTime[i]) = std::chrono::duration<float>(std::uniform_real_distribution<float>(0.f, std::get<2>(m_ObjectSpawnDelayTime[i]))(m_RandomAlgorithm));
				std::get<0>(m_ObjectSpawnDelayTime[i]) = std::chrono::system_clock::now();
			}
		}
	}
}