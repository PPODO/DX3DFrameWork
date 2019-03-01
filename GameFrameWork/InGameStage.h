#pragma once
#include "StageClass.h"
#include "EnemyClass.h"
#include "BackGroundUIClass.h"
#include <vector>
#include <utility>
#include <string>
#include <stack>
#include <random>

enum ENEMYSTYLE;

class InGameStage : public StageClass {
private:
	static std::random_device m_RandomDeivce;
	static std::mt19937_64 m_RandomAlgorithm;
	static std::uniform_int_distribution<int> m_Random;

	static class PlayerClass* m_Player;
	static class ObjectPoolClass* m_TempPoolManager;

private:
	std::vector<std::stack<class EnemyClass*>> m_Enemys;
	std::vector<class EnemyClass*> m_ActivatedEnemy;
	
	std::pair<short, short> m_EnemyStylePercentage[EnemyStyleCount];
	size_t m_LimitiedNumberOfSpawn;

private:
	void PickEnemyStyleAndSpawn();

	inline void ProcessEnemyActivity();
	inline ENEMYSTYLE CheckPercentage(int);

protected:
	virtual void ReleaseForChangingStage() override;

public:
	InGameStage(std::vector<StageClass*>&, int);
	virtual ~InGameStage() override;

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void ChangeStageNotification() override;

public:
	inline void SetObjectPoolAndPlayerClass(class ObjectPoolClass* OP, class PlayerClass* PC) { m_TempPoolManager = OP; m_Player = PC;}

};

inline ENEMYSTYLE InGameStage::CheckPercentage(int Percentage) {
	for (int i = EN_DEFAULT; i < EnemyStyleCount; i++) {
		if (m_EnemyStylePercentage[i].first <= Percentage && Percentage <= m_EnemyStylePercentage[i].second) {
			return (ENEMYSTYLE)i;
		}
	}
	return EN_DEFAULT;
}

inline void InGameStage::ProcessEnemyActivity() {
	if (!m_bNotificationForStop && m_ActivatedEnemy.size() < m_LimitiedNumberOfSpawn) {
		PickEnemyStyleAndSpawn();
	}
	else if (m_bNotificationForStop) {
		m_ActivatedEnemy.size() < 1 ? m_bIsStop = true : 0;
	}
}