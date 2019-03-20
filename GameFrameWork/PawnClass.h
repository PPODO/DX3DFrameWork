#pragma once
#include "Actor.h"

static const float Gravity = 0.5f;

class PawnClass : public Actor {
private:
	static RECT m_WindowSize;

private:
	float m_CurrentHeight, m_MaxHeight, m_SaveYLocation;
	bool m_bUseGravity, m_bLanded, m_bIsJumping, m_bStartMoveToLocation;

protected:
	float m_LocationIsHaveToGo;

protected:
	inline void DoJump();
	inline void CalculateJump();
	inline void IsLanded() { m_bLanded = true; GetActorImage()->SetPosition(D3DXVECTOR3(GetActorImage()->GetPosition().x, m_SaveYLocation, 0.f)); };

protected:
	virtual void SetupPlayerInput() = 0;
	virtual void PlayStartMoveToLocation() = 0;

public:
	PawnClass();
	virtual ~PawnClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;

public:
	inline void SetStartMoveToLocation(bool Value) { m_bStartMoveToLocation = Value; }

public:
	inline RECT GetWindowSize() const { return m_WindowSize; }
	inline bool GetIsJumping() const { return m_bIsJumping; }
	inline bool GetIsLanded() const { return m_bLanded; }
	inline bool GetUseGravity() const { return m_bUseGravity; }
	inline bool GetStartMoveToLocation() const { return m_bStartMoveToLocation; }

};

inline void PawnClass::CalculateJump() {
	if (m_bUseGravity && !m_bLanded) {
		if (m_bIsJumping && GetActorImage()->GetPosition().y + m_CurrentHeight > m_SaveYLocation) {
			m_CurrentHeight = 0.f;
			m_bIsJumping = false;
		}
		m_CurrentHeight += Gravity;
		GetActorImage()->AddYPosition(m_CurrentHeight);
	}
}

inline void PawnClass::DoJump() {
	if (m_bLanded && GetActorImage()) {
		m_bLanded = false;
		m_bIsJumping = true;
		m_CurrentHeight = m_MaxHeight;
		m_SaveYLocation = GetActorImage()->GetPosition().y;
	}
}