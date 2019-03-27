#pragma once
#include "Actor.h"

class ItemClass : public Actor {
private:


public:
	ItemClass();
	virtual ~ItemClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;

};