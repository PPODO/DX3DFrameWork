#pragma once
#include "ProjectileClass.h"

class NuclearProjectileClass : public ProjectileClass {
private:
	D3DXMATRIX m_Matrix, m_CenterLocation, m_Rotation, m_Location, m_DefaultMaxtrix;
	float m_Height;
	float m_SaveDeltaTime;
	D3DXVECTOR3 m_PrevLocation;

protected:
	virtual void ProjectileMovementProcessing(const float& DeltaTime) override;

public:
	NuclearProjectileClass();
	virtual ~NuclearProjectileClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;

};