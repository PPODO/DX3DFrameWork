#pragma once
#include "Actor.h"
#include "AnimationClass.h"

class ParticleClass : public Actor, public AnimationClass {
private:

public:
	ParticleClass();
	virtual ~ParticleClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;

public:
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f });

};