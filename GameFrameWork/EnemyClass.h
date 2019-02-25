#pragma once
#include "Pawn.h"

class EnemyClass : public Pawn {
private:


protected:
	virtual void ProcessEnemyMovement() = 0;

public:
	EnemyClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
};