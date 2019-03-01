#pragma once
#include "EnemyClass.h"

class SplitEnemy : public EnemyClass {
	enum ImageOrder { FIRST, SECOND };
	enum SplitState { SPLIT_NONE, SPLIT_SPLIT };
private:
	std::chrono::system_clock::time_point Point;
	class TextureClass* m_SplitImage;
	bool m_bIsReflect[2];
	SplitState m_States;

	D3DXVECTOR3 m_NormalVector[2], m_BeforePosition[2], m_ReflectDirection[2];

private:
	virtual inline bool CheckOutOfScreen() override;

	inline void CalculateReflectDirection(class TextureClass*, ImageOrder);
	inline bool YScreenCrash(class TextureClass*, D3DXVECTOR3&);
	inline void CheckSplitState(class TextureClass*, ImageOrder);

protected:
	virtual void EnemyMoveProcessing() override;

public:
	SplitEnemy();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual void ClearObject() override;
};

inline bool SplitEnemy::CheckOutOfScreen() {
	if (EnemyClass::CheckOutOfScreen()) {
		if (LONG(m_SplitImage->GetPosition().x + m_SplitImage->GetImageCenter().x) < GetWindowSize().left) {
			SetOutOfScreen(true);
		}
		else {
			SetOutOfScreen(false);
		}
	}
	return GetOutOfScreen();
}

inline void SplitEnemy::CalculateReflectDirection(class TextureClass* Texture, ImageOrder Order) {
	m_ReflectDirection[Order] = Texture->GetPosition() - m_BeforePosition[Order];
	D3DXVECTOR3 InverseDirection = m_BeforePosition[Order] - Texture->GetPosition();
	float Dot = D3DXVec3Dot(&InverseDirection, &m_NormalVector[Order]);

	m_ReflectDirection[Order] += (2 * m_NormalVector[Order] * Dot);
	D3DXVec3Normalize(&m_ReflectDirection[Order], &m_ReflectDirection[Order]);
	m_bIsReflect[Order] = true;
}

inline bool SplitEnemy::YScreenCrash(class TextureClass* Texture, D3DXVECTOR3& Direction) {
	if (LONG(Texture->GetPosition().y + Texture->GetImageCenter().y) >= GetWindowSize().bottom) {
		Direction = D3DXVECTOR3(0.f, 1.f, 0.f);
		return true;
	}
	else if (LONG(Texture->GetPosition().y - Texture->GetImageCenter().y) <= GetWindowSize().top) {
		Direction = D3DXVECTOR3(0.f, -1.f, 0.f);
		return true;
	}
	return false;
}

inline void SplitEnemy::CheckSplitState(class TextureClass* Texture, ImageOrder Order) {
	switch (m_States) {
	case SPLIT_NONE:
		Texture->AddXPosition(m_XMoveSpeed * -1);
		break;
	case SPLIT_SPLIT:
		if (m_bIsReflect[Order]) {
			Texture->AddPosition(m_ReflectDirection[Order] * (m_XMoveSpeed + m_YMoveSpeed / 2));
		}
		else {
			Texture->AddPosition((D3DXVECTOR3(1.f, (Order == FIRST ? -0.5f : 0.5f), 0.f)) * -(m_XMoveSpeed + m_YMoveSpeed / 1.5f));
		}
		break;
	}
}