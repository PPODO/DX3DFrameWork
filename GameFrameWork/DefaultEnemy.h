#pragma once
#include "EnemyClass.h"

class DefaultEnemy : public EnemyClass {
private:
	float m_Movement;

private:
	virtual void EnemyMovementProcessing() override;

public:
	DefaultEnemy();
	virtual ~DefaultEnemy() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;

};