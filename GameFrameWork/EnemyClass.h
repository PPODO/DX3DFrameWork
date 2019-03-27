#pragma once
#include "PawnClass.h"
#include <vector>
#include <stack>

enum EEnemyType { EET_DEFAULT, EET_COUNT };

class EnemyClass : public PawnClass {
private:
	class ObjectPoolClass* m_PoolManager;

private:
	virtual void SetupPlayerInput() {};
	virtual void PlayStartMoveToLocation(float Value = 0.05f) {};
	virtual void EnemyMovementProcessing() = 0;

protected:
	inline virtual void TimerNotification() override;
	inline class ObjectPoolClass* GetPoolManager() const { return m_PoolManager; }

public:
	EnemyClass();
	virtual ~EnemyClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual bool IsItOutOfScreen() override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;
	virtual void ClearObject() override;

public:
	inline void SetPoolManager(class ObjectPoolClass* OP) { m_PoolManager = OP; }

};

inline void EnemyClass::TimerNotification() {
	SetActorIsActivated(true);
}