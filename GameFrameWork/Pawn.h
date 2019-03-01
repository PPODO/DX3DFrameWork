#pragma once
#include "Actor.h"
#include "TextureClass.h"
#include <chrono>
#include <stack>

enum ScreenCoord { XSCREEN, YSCREEN };

class Pawn : public Actor {
protected:
	enum ProjectileStyle { PS_DEFAULT, PS_LASER, PS_HOMING, PS_COUNT };
private:
	RECT m_WindowSize;

protected:
	std::vector<std::stack<class ProjectileClass*>> m_Projectiles;
	std::vector<class ProjectileClass*> m_ActivedProjectiles;
	std::chrono::duration<float> m_FireDelay;
	std::chrono::system_clock::time_point m_LastFireTime;

	bool m_bIsMaximallyActive;
	size_t m_CurrentActivatedProjectile, m_MaxActivatedProjectile;
	ProjectileStyle m_CurrentProjectileStyle;

	float m_XMoveSpeed;
	float m_YMoveSpeed;

protected:
	inline bool CheckOutOfScreen(ScreenCoord, LONG);

public:
	Pawn();
	virtual ~Pawn();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void SetupPlayerInput();
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

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