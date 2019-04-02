#pragma once
#include "ProjectileClass.h"
#include "ParticleClass.h"

class BombProjectile : public ProjectileClass {
private:
	D3DXMATRIX m_Matrix, m_CenterLocation, m_Rotation, m_Location, m_DefaultMaxtrix;
	D3DXVECTOR3 m_PrevLocation;

private:
	float m_Height;
	float m_SaveDeltaTime;

private:
	ParticleClass* m_ExplosionEffect;

protected:
	virtual void ProjectileMovementProcessing(const float& DeltaTime) override;

public:
	BombProjectile();
	virtual ~BombProjectile() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void ClearObject() override;

};