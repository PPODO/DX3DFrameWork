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
	RECT m_WindowSize;

protected:
	std::vector<std::stack<class ProjectileClass*>> m_Projectiles;
	std::vector<class ProjectileClass*> m_ActivedProjectiles;
	std::chrono::duration<float> m_FireDelay;
	std::chrono::system_clock::time_point m_LastFireTime;

	size_t m_MaxActivatedProjectile;
	ProjectileStyle m_CurrentProjectileStyle;

	float m_XMoveSpeed;
	float m_YMoveSpeed;

protected:
	void SpawnProjectile(D3DXVECTOR3&&);
	
	inline void ClearProjectilePool();
	inline bool CheckOutOfScreen(ScreenCoord, LONG);
	virtual bool IsProjectileOutOfScreen(class ProjectileClass*) = 0;

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

	bool IsCrashed(const Actor* Object);

public:
	inline RECT GetWindowSize() const { return m_WindowSize; }
	TextureClass* GetTexture() const { return m_Texture; }
};

inline bool Pawn::CheckOutOfScreen(ScreenCoord SC, LONG Value) {
	if (SC == XSCREEN) {
		if ((LONG)m_Texture->GetPosition().x + m_Texture->GetImageCenter().x + Value <= m_WindowSize.right && (LONG)m_Texture->GetPosition().x - m_Texture->GetImageCenter().x + Value >= m_WindowSize.left) {
			return true;
		}
		return false;
	}
	if ((LONG)m_Texture->GetPosition().y + m_Texture->GetImageCenter().y + Value <= m_WindowSize.bottom && (LONG)m_Texture->GetPosition().y - m_Texture->GetImageCenter().y + Value >= m_WindowSize.top) {
		return true;
	}
	return false;
}