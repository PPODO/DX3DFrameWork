#pragma once
#include "ProjectileClass.h"
#include "AnimationClass.h"
#include "Actor.h"
#include <chrono>
#include <vector>
#include <stack>

class PawnClass : public Actor {
private:
	static RECT m_WindowSize;

private:
	AnimationClass* m_PawnAnim;
	class ObjectPoolClass* m_PoolManager;
	TextureClass* m_CopyImage;

protected:
	int m_SpriteWidth, m_SpriteHeight;

protected:
	virtual void SpawnExplosionEffect();

private:
	bool m_bIsFire;
	std::chrono::system_clock::time_point m_LastFireTime;

protected:
	std::vector<std::stack<ProjectileClass*>> m_ProjectileObjects;
	std::vector<ProjectileClass*> m_ActivatedProjectileObjects;

protected:
	EProjectileType m_ProjectileType;
	size_t m_MaxActivatedProjectile;
	std::chrono::duration<float> m_FireDelayTime;

protected:
	inline bool CanFire();

protected:
	class ParticleClass* m_ExplosionEffect;

private:
	float m_MaxHeight;
	bool m_bUseGravity, m_bLanded, m_bIsJumping, m_bStartMoveToLocation;

protected:
	float m_SaveYLocation, m_CurrentHeight;

protected:
	float m_LocationIsHaveToGo;

protected:
	inline void DoJump();
	inline void ClearJump();
	inline void CalculateJump();
	inline void IsLanded() { m_bLanded = true; GetActorImage()->SetPosition(D3DXVECTOR3(GetActorImage()->GetPosition().x, m_SaveYLocation, 0.f)); }

protected:
	virtual void SetupPlayerInput() = 0;
	virtual void PlayStartMoveToLocation(float Value = 0.05f) = 0;
	ProjectileClass* FireProjectile(const D3DXVECTOR3& Offset = { 0.f, 0.f, 0.f });
	void CalculateProjectile(const float& DeltaTime, float ActorHeight);

public:
	PawnClass();
	virtual ~PawnClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;

public:
	inline void SetCurrentHeight(float Value) { m_CurrentHeight = Value; }
	inline void SetStartMoveToLocation(bool Value) { m_bStartMoveToLocation = Value; }
	inline void SetFireState(bool b) { m_bIsFire = b; }
	inline void SetPoolManager(class ObjectPoolClass* OP) { m_PoolManager = OP; }

public:
	inline bool GetFireState() const { return m_bIsFire; }
	inline RECT GetWindowSize() const { return m_WindowSize; }
	inline bool GetIsJumping() const { return m_bIsJumping; }
	inline bool GetIsLanded() const { return m_bLanded; }
	inline bool GetUseGravity() const { return m_bUseGravity; }
	inline bool GetStartMoveToLocation() const { return m_bStartMoveToLocation; }
	inline float GetCurrentHeight() const { return m_CurrentHeight; }
	inline float GetMaxHeight() const { return m_MaxHeight; }
	inline class ObjectPoolClass* GetPoolManager() const { return m_PoolManager; }

};

inline void PawnClass::CalculateJump() {
	if (m_bUseGravity && !m_bLanded) {
		if (m_bIsJumping && GetActorImage()->GetPosition().y + m_CurrentHeight > m_SaveYLocation) {
			m_CurrentHeight = 0.f;
			m_bIsJumping = false;
		}
		else {
			m_CurrentHeight += Gravity;
			GetActorImage()->AddYPosition(m_CurrentHeight);
		}
	}
} 

inline bool PawnClass::CanFire() {
	auto CurTime = std::chrono::system_clock::now();
	if (CurTime - m_LastFireTime > m_FireDelayTime) {
		m_LastFireTime = CurTime;
		return true;
	}
	return false;
}

inline void PawnClass::DoJump() {
	if (m_bLanded && GetActorImage()) {
		m_bLanded = false;
		m_bIsJumping = true;
		m_CurrentHeight = m_MaxHeight;
		m_SaveYLocation = GetActorImage()->GetPosition().y;
	}
}

inline void PawnClass::ClearJump() {
	m_bIsJumping = false;
	m_bLanded = true;
	m_CurrentHeight = 0.f;
}