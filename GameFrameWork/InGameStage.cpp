#include "InGameStage.h"
#include "EnemyClass.h"
#include "ObjectPoolClass.h"
#include "PlayerClass.h"

std::random_device InGameStage::m_RandomDeivce;
std::mt19937_64 InGameStage::m_RandomAlgorithm;
std::uniform_int_distribution<int> InGameStage::m_Random;

PlayerClass* InGameStage::m_Player = nullptr;
ObjectPoolClass* InGameStage::m_TempPoolManager = nullptr;

InGameStage::InGameStage(std::vector<StageClass*>& Vectors, int NumberOfSpawn) : StageClass(Vectors), m_LimitiedNumberOfSpawn(NumberOfSpawn), m_GameStart(false) {
	m_Enemys.resize(3);

	m_EnemyStylePercentage[EN_DEFAULT] = std::make_pair(0, 0);
	m_EnemyStylePercentage[EN_RUSH] = std::make_pair(1, 10);
	m_EnemyStylePercentage[EN_SPLIT] = std::make_pair(0, 10);
/*	m_EnemyStylePercentage[EN_FLIGHT] = std::make_pair(0, 10);	*/

	m_Counting = 3;
	m_StageTime = std::chrono::duration<float>(5.f);
	m_RandomAlgorithm = std::mt19937_64(m_RandomDeivce());
}

InGameStage::~InGameStage() {
	if (m_TempPoolManager) {
		for (auto Iterator : m_Enemys) {
			if (Iterator.size() > 0) {
				m_TempPoolManager->ReleaseAll(Iterator.top()->GetName(), Iterator, Iterator.size());
			}
		}
		for (auto Iterator : m_ActivatedEnemy) {
			m_TempPoolManager->Release(Iterator->GetName(), Iterator);
		}
	}
	m_ActivatedEnemy.clear();
}

bool InGameStage::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_BackGround = new BackGroundUIClass;
	if (!m_BackGround) {
		return false;
	}
	m_BackGround->Init(Device, true, L"Stage1.png", L"Stage1.png");

	(new TextClass(m_Text))->Init(Device, L"Count1.png");
	(new TextClass(m_Text))->Init(Device, L"Count2.png");
	(new TextClass(m_Text))->Init(Device, L"Count3.png");
	if (m_Text.empty()) {
		return false;
	}

	m_PlayerStartPosition = D3DXVECTOR3(100.f, (GetWindowSize().bottom / 2) - (m_Player ? m_Player->GetTexture()->GetImageCenter().y : 25.f), 0.f);
	return true;
}

void InGameStage::Update(float DeltaTime) {
	StageClass::Update(DeltaTime);

	if (m_GameStart) {
		if (m_Player) {
			m_Player->Update(DeltaTime);
		}

		for (auto It = m_ActivatedEnemy.begin(); It != m_ActivatedEnemy.end();) {
			if ((*It) && !(*It)->GetIsDead()) {
				(*It)->Update(DeltaTime);
				It++;
			}
			else {
				(*It)->PoolThisObject(It);
			}
		}
		ProcessEnemyActivity();
	}
	else {
		if (m_Player) {
			if (m_Player->MoveToPosition(m_PlayerStartPosition, DeltaTime)) {
				StartCounting();
			}
		}
	}
}

void InGameStage::Render(LPD3DXSPRITE Sprite) {
	StageClass::Render(Sprite);

	if (m_Player) {
		m_Player->Render(Sprite);
	}

	if (m_GameStart) {
		for (auto It : m_ActivatedEnemy) {
			It->Render(Sprite);
		}
	}
	else {

	}
}

void InGameStage::Destroy() {
	StageClass::Destroy();
	for (auto& It : m_Text) {
		It->Destroy();
		delete It;
	}
	m_Text.clear();
}

void InGameStage::PickEnemyStyleAndSpawn() {
	m_Random = std::uniform_int_distribution<int>(0, 10);

	for (size_t i = m_ActivatedEnemy.size(); i < m_LimitiedNumberOfSpawn; i++) {
		int Percentage = m_Random(m_RandomAlgorithm);
		ENEMYSTYLE EnemyStyle = CheckPercentage(Percentage);

		EnemyClass* EC = m_Enemys[EnemyStyle].top();
		m_Enemys[EnemyStyle].pop();
		if (EC) {
			EC->SpawnObject();
			m_ActivatedEnemy.push_back(EC);
		}
	}
}

void InGameStage::ChangeStageNotification() {
	if (m_TempPoolManager) {
		m_TempPoolManager->GetPoolObject("DefaultEnemy", m_Enemys[EN_DEFAULT], m_LimitiedNumberOfSpawn);
		m_TempPoolManager->GetPoolObject("RushEnemy", m_Enemys[EN_RUSH], m_LimitiedNumberOfSpawn);
		m_TempPoolManager->GetPoolObject("SplitEnemy", m_Enemys[EN_SPLIT], m_LimitiedNumberOfSpawn);
		
		m_Enemys[EN_DEFAULT].top()->SetPoolingList(&m_Enemys, &m_ActivatedEnemy);
		m_Enemys[EN_DEFAULT].top()->SetTarget(m_Player->GetTexture());
	}
	m_LastCountTime = std::chrono::system_clock::now();
}

void InGameStage::ReleaseForChangingStage() {
	if (m_TempPoolManager) {
		for (auto& Iterator : m_Enemys) {
			if (Iterator.size() > 0) {
				Iterator.top()->SetPoolingList(nullptr, nullptr);
				m_TempPoolManager->ReleaseAll(Iterator.top()->GetName(), Iterator, Iterator.size());
			}
		}
	}
	m_Enemys.clear();

	if (m_Player) {
		m_Player->ResetAllObject();
	}
}

void InGameStage::StartCounting() {
	auto CurrentTime = std::chrono::system_clock::now();
	std::chrono::seconds Second = std::chrono::duration_cast<std::chrono::seconds>(CurrentTime - m_LastCountTime);

	if (Second.count() >= 1) {
		if (--m_Counting <= 0) {
			if (m_Player) {
				m_Player->SetCanMove(true);
			}
			m_bUseTimer = true;
			StartTimer();
			m_GameStart = true;
		}
		m_LastCountTime = CurrentTime;
	}
}