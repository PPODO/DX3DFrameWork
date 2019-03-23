#pragma once
#include "ProjectileClass.h"

class DefaultProjectile : public ProjectileClass {
private:
	D3DXMATRIX m_Matrix, m_CenterLocation, m_Rotation, m_Location, m_DefaultMaxtrix;
	D3DXVECTOR3 m_PrevLocation;
	float m_SaveDeltaTime;
	float m_Height;

protected:
	virtual void ProjectileMovementProcessing(const float& DeltaTime) override;

public:
	DefaultProjectile();
	virtual ~DefaultProjectile() override;
	
public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;

};