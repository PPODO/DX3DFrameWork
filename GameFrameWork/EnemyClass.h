#pragma once
#include "PawnClass.h"
#include <vector>
#include <stack>

enum EEnemyType { EET_DEFAULT, EET_COUNT };

class EnemyClass : public PawnClass {
private:
	std::vector<std::stack<class ProjectileClass*>>* m_ProjectileObjects;
	std::vector<std::pair<class ProjectileClass*, size_t>> m_ActivatedProjectile;

protected:
	EProjectileType m_ProjectileType;

private:
	virtual void SetupPlayerInput() {};
	virtual void PlayStartMoveToLocation(float Value = 0.05f) {};
	virtual void EnemyMovementProcessing() = 0;

protected:
	inline virtual void TimerNotification() override;
	virtual ProjectileClass* FireProjectile(const D3DXVECTOR3& Offset = { 0.f, 0.f, 0.f }) override;
	virtual void CalculateProjectile(const float& DeltaTime) override;

public:
	EnemyClass();
	virtual ~EnemyClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;
	
public:
	void SetProjectileObject(std::vector<std::stack<class ProjectileClass*>>& List);
	void ReleaseProjectileObject();

};

inline void EnemyClass::TimerNotification() {
	SetActorIsActivated(true);
}