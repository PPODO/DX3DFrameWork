#pragma once
#include "Actor.h"

class BackGroundClass : public Actor {
private:

private:
	void BackGroundMovementProcessing();

public:
	BackGroundClass();
	virtual ~BackGroundClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc);
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void CollisionEventBeginByOtherActor(Actor*) override;

};