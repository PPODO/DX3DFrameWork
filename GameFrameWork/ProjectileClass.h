#pragma once
#include "Actor.h"
#include "ParticleClass.h"

enum EProjectileType { EPT_DEFAULT, EPT_BOMB, EPT_HOMINGMISSILE, EPT_NUCLEAR, EPT_COUNT };

class ProjectileClass : public Actor {
private:
	Actor* m_Owner;

protected:
	Actor* m_Target;
	bool m_bIsExplosion;
	ParticleClass* m_ExplosionEffect;

protected:
	EProjectileType m_ProjectileType;
	D3DXVECTOR3 m_MoveDirection;
	float m_Seta;

protected:
	virtual void ProjectileMovementProcessing(const float& DeltaTime) = 0;

public:
	ProjectileClass();
	virtual ~ProjectileClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f }) override;
	virtual void ClearObject() override;

public:
	EProjectileType GetProjectileType() const { return m_ProjectileType; }
	Actor* GetOwner() const { return m_Owner; }

	void SetMoveDirection(const D3DXVECTOR3& Vec) { m_MoveDirection = Vec; D3DXVec3Normalize(&m_MoveDirection, &m_MoveDirection); }
	void SetProjectileSeta(const float& Seta) { m_Seta = Seta; }
	void SetTargetActor(Actor* TargetActor) { m_Target = TargetActor; };
	void SetOwner(Actor* Owner) { m_Owner = Owner; }

};