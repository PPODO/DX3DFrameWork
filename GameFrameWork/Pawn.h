#pragma once
#include "Actor.h"
#include "TextureClass.h"
#include "ObjectPoolClass.h"
#include "ProjectileClass.h"
#include <chrono>
#include <stack>
#include <vector>

enum ScreenCoord { XSCREEN, YSCREEN };

enum ProjectileStyle;

const float VelocityY = -5.0f;
const float Gravity = 0.5f;

class Pawn : public Actor {
private:
	std::chrono::system_clock::time_point m_LastFireTime;
	bool m_bIsJumping, m_bIsFalling, m_Landed;
	float m_CurrentVelocityY;

protected:
	static class ObjectPoolClass* m_PoolManager;

protected:
	float m_XMoveSpeed, m_YMoveSpeed;

protected:
	std::vector<std::stack<class ProjectileClass*>> m_Projectiles;
	std::vector<class ProjectileClass*> m_ActivatedProjectile;

	std::chrono::duration<float> m_FireDelay;

	ProjectileStyle m_CurrentProjectileStyle;
	size_t m_MaxActiveProjectile;

protected:
	void ClearProjectile();
	void ClearProjectilePool();

	void SpawnProjectile(const D3DXVECTOR3&);

public:
	Pawn();
	virtual ~Pawn();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void SetupPlayerInput();
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

public:
	void SetJumping(bool b) { if (!m_bIsJumping) { m_bIsJumping = b; m_CurrentVelocityY = VelocityY; m_Landed = false; } }
	void SetLanded(bool b) { if (m_bIsFalling) { m_Landed = b; m_bIsJumping = m_bIsFalling = false; } }


};