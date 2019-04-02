#pragma once
#include "ProjectileClass.h"

class HomingMissileProjectile : public ProjectileClass {
private:
	D3DXMATRIX m_Matrix, m_CenterLocation, m_Rotation, m_Location, m_DefaultMaxtrix;

protected:
	virtual void ProjectileMovementProcessing(const float& DeltaTime) override;

public:
	HomingMissileProjectile();
	virtual ~HomingMissileProjectile() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;

};