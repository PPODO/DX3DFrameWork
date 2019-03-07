#include "InGameStage.h"
#include "EnemyClass.h"
#include "BackGroundClass.h"

ObjectPoolClass* InGameStage::m_PoolManager = nullptr;
PlayerClass* InGameStage::m_Player = nullptr;

InGameStage::InGameStage(std::vector<StageClass*>& m_Vector, size_t MaxSpawn) : StageClass(m_Vector), m_MaxSpawn(MaxSpawn) {
	m_Enemys.resize(EnemyStyleCount);
}

InGameStage::~InGameStage() {
}

bool InGameStage::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_BackGround = new BackGroundClass;
	if (!m_BackGround) {
		return false;
	}
	m_BackGround->Init(Device, L"Stage/Stage1.png", L"Stage/Stage1.png");
	m_BackGround->SetBackGroundMovement(true, 5.f, -1);

	m_Ground = new BackGroundClass;
	if (!m_Ground) {
		return false;
	}
	m_Ground->Init(Device, L"Stage/Stage1_Ground.png", L"Stage/Stage1_Ground.png");
	m_Ground->SetBackGroundMovement(true, 2.f, -1);
	m_Ground->SetPosition(D3DXVECTOR3(0.f, FLOAT(GetWindowSize().bottom - m_Ground->GetActorTexture(0)->GetRect().bottom), 0.f));
	m_Ground->SetCollisionTypeAndBindEvent(CT_ALLBLOCK);

	m_BackMove = new BackGroundClass;
	if (!m_BackMove) {
		return false;
	}
	m_BackMove->Init(Device, L"Stage/Stage1_Ground2.png", L"Stage/Stage1_Ground2.png");
	m_BackMove->SetBackGroundMovement(true, 3.f, -1);
	m_BackMove->SetPosition(D3DXVECTOR3(0.f, FLOAT(GetWindowSize().bottom - m_Ground->GetActorTexture(0)->GetRect().bottom - m_BackMove->GetActorTexture(0)->GetRect().bottom), 0.f));

	
	return true;
}

void InGameStage::Update(float DeltaTime) {
	StageClass::Update(DeltaTime);

	m_BackMove->Update(DeltaTime);
	m_Ground->Update(DeltaTime);

	if (m_Player) {
		m_Player->Update(DeltaTime);
	}

/*	for (auto It = m_ActivatedEnemy.begin(); It != m_ActivatedEnemy.end();) {
		if ((*It) && (*It)->GetIsActivation()) {
			(*It)->Update(DeltaTime);
			++It;
		}
		else {
			(*It)->PoolThisObject(m_Enemys[m_PoolManager->GetKeyByObjectName((*It)->GetName())], m_ActivatedEnemy, It);
		}
	}

	PickEnemyStyleAndSpawn();*/
}

void InGameStage::Render(LPD3DXSPRITE Sprite) {
	StageClass::Render(Sprite);

	m_BackMove->Render(Sprite);
	m_Ground->Render(Sprite);

	if (m_Player) {
		m_Player->Render(Sprite);
	}
	
	for (const auto& It : m_ActivatedEnemy) {
		if (It) {
			It->Render(Sprite);
		}
	}
}

void InGameStage::Destroy() {
	StageClass::Destroy();

	if (m_BackMove) {
		m_BackMove->Destroy();
		delete m_BackMove;
		m_BackMove = nullptr;
	}

	if (m_Ground) {
		m_Ground->Destroy();
		delete m_Ground;
		m_Ground = nullptr;
	}

	ClearEnemyObjects();
	m_Player = nullptr;
}

void InGameStage::PickEnemyStyleAndSpawn() {
	for (size_t i = m_ActivatedEnemy.size(); i < m_MaxSpawn; i++) {
		auto Enemy = m_Enemys[ES_DEFAULT].top();
		if (Enemy) {
			Enemy->SpawnObject();
			m_ActivatedEnemy.push_back(Enemy);
			m_Enemys[ES_DEFAULT].pop();
		}
	}
}

void InGameStage::ChangeStageNotification() {
	if (m_PoolManager) {
		m_PoolManager->GetPoolObject("DefaultEnemy", m_Enemys[ES_DEFAULT], m_MaxSpawn);
	}
}

void InGameStage::ReleaseForChangingStage() {
	ClearEnemyObjects();

}

void InGameStage::ClearEnemyObjects() {
	if (m_PoolManager) {
		for (auto& Iterator : m_ActivatedEnemy) {
			m_Enemys[m_PoolManager->GetKeyByObjectName(Iterator->GetName())].push(Iterator);
		}
		m_ActivatedEnemy.clear();

		for (auto& Iterator : m_Enemys) {
			if (Iterator.size() > 0) {
				m_PoolManager->ReleaseAll(Iterator.top()->GetName(), Iterator, Iterator.size());
			}
		}
		m_Enemys.clear();
	}
}