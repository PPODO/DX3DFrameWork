#pragma once
#include "EnemyClass.h"

class RushEnemyClass : public EnemyClass {
private:
	bool m_bSetDirection;
	D3DXVECTOR3 MoveDirection;

private:
	void CalculateDirection();

protected:
	virtual void EnemyMoveProcessing() override;
	virtual void ClearObject() override;

public:
	RushEnemyClass();
	virtual ~RushEnemyClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
};