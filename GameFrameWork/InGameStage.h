#pragma once
#include "StageClass.h"
#include <vector>
#include <utility>
#include <string>
#include <stack>
#include <random>

class InGameStage : public StageClass {
	enum ENEMYSTYLE { EN_DEFAULT, EN_RUSH, EN_SPLIT, EN_FLIGHT, EnemyStyleCount };
private:
	static std::random_device m_RandomDeivce;
	static std::mt19937_64 m_RandomAlgorithm;
	static std::uniform_int_distribution<int> m_Random;

private:
	std::vector<std::stack<class EnemyClass*>> m_Enemys;
	std::vector<class EnemyClass*> m_ActivatedEnemy;
	class ObjectPoolClass* m_TempPoolManager;

	std::pair<short, short> m_EnemyStylePercentage[EnemyStyleCount];
	size_t m_LimitiedNumberOfSpawn;

private:
	void PickEnemyStyleAndSpawn();
	void ReleaseActivatedObject(std::vector<EnemyClass*>::iterator& It);
	inline ENEMYSTYLE FindEnemyStyleUsingString(const std::string& Name);
	inline ENEMYSTYLE CheckPercentage(int);

protected:
	virtual void ReleaseForChangingStage() override;

public:
	InGameStage(std::vector<StageClass*>&, class ObjectPoolClass*, int);
	virtual ~InGameStage() override;

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void ChangeStageNotification() override;
};

inline InGameStage::ENEMYSTYLE InGameStage::CheckPercentage(int Percentage) {
	for (int i = EN_DEFAULT; i < EnemyStyleCount; i++) {
		if (m_EnemyStylePercentage[i].first <= Percentage && Percentage <= m_EnemyStylePercentage[i].second) {
			return (ENEMYSTYLE)i;
		}
	}
	return EN_DEFAULT;
}

inline InGameStage::ENEMYSTYLE InGameStage::FindEnemyStyleUsingString(const std::string& Name) {
	if (Name.compare("DefaultEnemy") == 0) {
		return EN_DEFAULT;
	}
	else if (Name.compare("RushEnemy") == 0) {
		return EN_RUSH;
	}
	else if (Name.compare("SplitEnemy") == 0) {
		return EN_SPLIT;
	}
	else if (Name.compare("FlightEnemy") == 0) {
		return EN_FLIGHT;
	}
}