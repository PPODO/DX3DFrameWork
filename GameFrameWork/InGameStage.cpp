#include "InGameStage.h"
#include "ObjectPoolClass.h"
#include "PlayerClass.h"

std::random_device InGameStage::m_RandomDeivce;
std::mt19937_64 InGameStage::m_RandomAlgorithm;
std::uniform_int_distribution<int> InGameStage::m_Random;

PlayerClass* InGameStage::m_Player = nullptr;
ObjectPoolClass* InGameStage::m_TempPoolManager = nullptr;

InGameStage::InGameStage(std::vector<StageClass*>& Vectors, int NumberOfSpawn) : StageClass(Vectors), m_LimitiedNumberOfSpawn(NumberOfSpawn) {
	m_Enemys.resize(3);

	m_EnemyStylePercentage[EN_DEFAULT] = std::make_pair(0, 3);
	m_EnemyStylePercentage[EN_RUSH] = std::make_pair(3, 6);
	m_EnemyStylePercentage[EN_SPLIT] = std::make_pair(6, 10);
//	m_EnemyStylePercentage[EN_FLIGHT] = std::make_pair(0, 10);

	m_bUseTimer = true;
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
	
	return true;
}

void InGameStage::Update(float DeltaTime) {
	StageClass::Update(DeltaTime);

	if (m_Player) {
		m_Player->Update(DeltaTime);
	}

	for (auto Iterator = m_ActivatedEnemy.begin(); Iterator != m_ActivatedEnemy.end();) {
		if (!(*Iterator)->CheckOutOfScreen(Iterator)) {
			(*Iterator)->Update(DeltaTime);
			Iterator++;
		}
	}

	ProcessEnemyActivity();
}

void InGameStage::Render(LPD3DXSPRITE Sprite) {
	StageClass::Render(Sprite);

	if (m_Player) {
		m_Player->Render(Sprite);
	}

	for (auto It : m_ActivatedEnemy) {
		It->Render(Sprite);
	}
}

void InGameStage::Destroy() {
	StageClass::Destroy();
}

void InGameStage::PickEnemyStyleAndSpawn() {
	m_Random = std::uniform_int_distribution<int>(m_ActivatedEnemy.size(), m_LimitiedNumberOfSpawn);

	for (size_t i = m_Random.a(); i < m_LimitiedNumberOfSpawn; i++) {
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
		m_Enemys[EN_DEFAULT].top()->SetTargetPosition(m_Player->GetTexture());
	}
	PickEnemyStyleAndSpawn();

	StartTimer();
}

void InGameStage::ReleaseForChangingStage() {
	if (m_TempPoolManager) {
		for (auto Iterator : m_Enemys) {
			if (Iterator.size() > 0) {
				Iterator.top()->SetPoolingList(nullptr, nullptr);
				m_TempPoolManager->ReleaseAll(Iterator.top()->GetName(), Iterator, Iterator.size());
			}
		}
	}
	m_Enemys.clear();
}