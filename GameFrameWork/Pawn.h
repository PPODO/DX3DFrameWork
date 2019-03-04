#pragma once
#include "Actor.h"
#include "TextureClass.h"
#include "ObjectPoolClass.h"
#include <chrono>
#include <stack>

enum ScreenCoord { XSCREEN, YSCREEN };

enum ProjectileStyle;

class Pawn : public Actor {
private:
	static ObjectPoolClass* m_PoolManager;

	std::chrono::system_clock::time_point m_LastFireTime;
	ProjectileStyle m_CurrentProjectileStyle;

protected:
	float m_Health;
	bool m_bIsDead;

	bool m_UseAutoSpawn;
	D3DXVECTOR3 m_ProjectileDirection;
	size_t m_MaxActivatedProjectile;
	std::chrono::duration<float> m_FireDelay;
	std::vector<std::stack<class ProjectileClass*>> m_Projectiles;
	std::vector<class ProjectileClass*> m_ActivedProjectiles;

	float m_XMoveSpeed;
	float m_YMoveSpeed;

protected:
	void ClearProjectilePool();

protected:
	void SetPoolManager(ObjectPoolClass* OP) { m_PoolManager = OP; }
	ObjectPoolClass* GetPoolManager() const { return m_PoolManager; }

public:
	Pawn();
	virtual ~Pawn();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void SetupPlayerInput();
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

	void SpawnProjectile(const D3DXVECTOR3&);
	void ClearActivatedProjectile();

public:
	virtual inline bool GetIsDead() const { return m_bIsDead; }

	void SetProjectiles(std::stack<class ProjectileClass*>&, size_t MaxListProjectile, float FireDelay, bool UseAuto = false, const D3DXVECTOR3& Direction = D3DXVECTOR3(0.f,0.f,0.f));

	void ApplyDamage(float f) { m_Health -= f; if (m_Health <= 0.f) { m_bIsDead = true; } }
	void SetIsDead(bool b) { m_bIsDead = b; }
	void SetHealth(float f) { m_Health = f; }
	inline float GetHealth() const { return m_Health; }

	inline size_t GetCurrentActivatedProjectiles() const { return m_ActivedProjectiles.size(); }
	TextureClass* GetTexture() const { return m_Texture; }
};