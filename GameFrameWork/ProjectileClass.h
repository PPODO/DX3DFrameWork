#pragma once
#include "Actor.h"

enum EProjectileType { EPT_DEFAULT, EPT_COUNT };

class ProjectileClass : public Actor {
protected:
	EProjectileType m_ProjectileType;
	float m_Seta;

protected:
	virtual void ProjectileMovementProcessing(const float& DeltaTime) = 0;

public:
	ProjectileClass();
	virtual ~ProjectileClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;

public:
	EProjectileType GetProjectileType() const { return m_ProjectileType; }
	void SetProjectileSeta(const float& Seta) { m_Seta = Seta; }

};