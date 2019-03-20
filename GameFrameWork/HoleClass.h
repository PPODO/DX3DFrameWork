#pragma once
#include "ObstacleClass.h"

class HoleClass : public ObstacleClass {
private:


protected:
	virtual void ObstacleMovementProcessing() override;

public:
	HoleClass();
	virtual ~HoleClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;

};