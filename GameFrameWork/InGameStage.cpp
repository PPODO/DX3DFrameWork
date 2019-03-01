#include "InGameStage.h"
#include "EnemyClass.h"
#include "BackGroundUIClass.h"
#include "ObjectPoolClass.h"

std::random_device InGameStage::m_RandomDeivce;
std::mt19937_64 InGameStage::m_RandomAlgorithm;
std::uniform_int_distribution<int> InGameStage::m_Random;

InGameStage::InGameStage(std::vector<StageClass*>& Vectors, ObjectPoolClass* OP, int NumberOfSpawn) : StageClass(Vectors), m_TempPoolManager(OP), m_LimitiedNumberOfSpawn(NumberOfSpawn) {
	m_Enemys.resize(1);
	
	//
	m_EnemyStylePercentage[0].first = 0;
	m_EnemyStylePercentage[0].second = 10;
	//

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

	for (auto Iterator = m_ActivatedEnemy.begin(); Iterator != m_ActivatedEnemy.end();) {
		(*Iterator)->Update(DeltaTime);
		ReleaseActivatedObject(Iterator);
	}

	if (!m_bNotificationForStop && m_ActivatedEnemy.size() < m_LimitiedNumberOfSpawn) {
		PickEnemyStyleAndSpawn();
	}
	else if (m_bNotificationForStop) {
		m_ActivatedEnemy.size() < 1 ? m_bIsStop = true : 0;
	}
}

void InGameStage::Render(LPD3DXSPRITE Sprite) {
	StageClass::Render(Sprite);

	for (auto It : m_ActivatedEnemy) {
		It->Render(Sprite);
	}
}

void InGameStage::Destroy() {
	StageClass::Destroy();
}

void InGameStage::ChangeStageNotification() {
	if (m_TempPoolManager) {
		//m_TempPoolManager->GetPoolObject("DefaultEnemy", m_Enemys[EN_DEFAULT], m_LimitiedNumberOfSpawn);
		m_TempPoolManager->GetPoolObject("SplitEnemy", m_Enemys[EN_DEFAULT], m_LimitiedNumberOfSpawn);
	}
	PickEnemyStyleAndSpawn();

	StartTimer();
}

void InGameStage::PickEnemyStyleAndSpawn() {
	m_Random = std::uniform_int_distribution<int>(m_ActivatedEnemy.size(), m_LimitiedNumberOfSpawn);
	for (size_t i = m_Random.a(); i < m_LimitiedNumberOfSpawn; i++) {
		int Percentage = m_Random(m_RandomAlgorithm);
		ENEMYSTYLE EnemyStyle = CheckPercentage(Percentage);

		EnemyClass* EC = m_Enemys[EnemyStyle].top();
		m_Enemys[EnemyStyle].pop();
		if (EC) {
			EC->SpawnAtLocation();
			m_ActivatedEnemy.push_back(EC);
		}
	}
}

void InGameStage::ReleaseActivatedObject(std::vector<EnemyClass*>::iterator& It) {
	if ((*It) && (*It)->GetOutOfScreen()) {
		ENEMYSTYLE EnemyStyle = FindEnemyStyleUsingString((*It)->GetName());
		(*It)->ClearObject();
		m_Enemys[0].push(*It);
		It = m_ActivatedEnemy.erase(It);
	}
	else {
		It++;
	}
}

void InGameStage::ReleaseForChangingStage() {
	if (m_TempPoolManager) {
		for (auto Iterator : m_Enemys) {
			if (Iterator.size() > 0) {
				m_TempPoolManager->ReleaseAll(Iterator.top()->GetName(), Iterator, Iterator.size());
			}
		}
	}
	m_Enemys.clear();
}