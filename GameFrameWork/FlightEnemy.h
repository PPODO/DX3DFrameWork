#pragma once
#include "EnemyClass.h"

class FlightEnemy : public EnemyClass {
private:

protected:
	virtual void EnemyMoveProcessing() override;

public:
	FlightEnemy();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
};