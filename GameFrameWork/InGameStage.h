#pragma once
#include "StageClass.h"
#include "PlayerClass.h"
#include "ObjectPoolClass.h"
#include <vector>
#include <stack>

class InGameStage : public StageClass {
private:
	static ObjectPoolClass* m_PoolManager;
	static PlayerClass* m_Player;

private:
	std::vector<std::stack<class EnemyClass*>> m_Enemys;
	std::vector<class EnemyClass*> m_ActivatedEnemy;
	size_t m_MaxSpawn;

	BackGroundClass* m_Ground;
	BackGroundClass* m_BackMove;

private:
	void ClearEnemyObjects();
	void PickEnemyStyleAndSpawn();

public:
	InGameStage(std::vector<StageClass*>&, size_t MaxSpawn);
	virtual ~InGameStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 });
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

	virtual void ChangeStageNotification() override;
	virtual void ReleaseForChangingStage() override;

public:
	void SetPlayerAndPoolManager(PlayerClass* PC, ObjectPoolClass* OP) { m_Player = PC, m_PoolManager = OP; }

};