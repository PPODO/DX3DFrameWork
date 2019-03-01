#pragma once
#include "EnemyClass.h"

class RushEnemy : public EnemyClass {
private:
	D3DXVECTOR3 m_MoveDirection;

protected:
	virtual void EnemyMoveProcessing() override;

public:
	RushEnemy();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void SpawnObject() override;
};