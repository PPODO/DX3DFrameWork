#pragma once
#include "EnemyClass.h"

class DefaultEnemy : public EnemyClass {
private:


private:
	virtual void EnemyMovementProcessing() override;

public:
	DefaultEnemy();
	virtual ~DefaultEnemy() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;

};