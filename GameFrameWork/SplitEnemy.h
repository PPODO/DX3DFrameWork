#pragma once
#include "EnemyClass.h"

class SplitEnemy : public EnemyClass {
	enum ImageOrder { FIRST, SECOND, IMAGECOUNT };
	enum SplitState { SPLIT_NONE, SPLIT_SPLIT };
private:
	class TextureClass* m_SplitImage;
	D3DXVECTOR3 m_NormalVector[IMAGECOUNT], m_BeforePosition[IMAGECOUNT], m_ReflectDirection[IMAGECOUNT];
	bool m_bIsReflect[IMAGECOUNT];
	SplitState m_States;

private:
	inline void CalculateReflectDirection(class TextureClass*, ImageOrder);
	inline bool CheckHeight(class TextureClass*, D3DXVECTOR3&);
	inline void CheckSplitState(class TextureClass*, ImageOrder);

protected:
	virtual void EnemyMoveProcessing() override;
	virtual void ClearObject() override;

public:
	SplitEnemy();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;
	virtual inline bool CheckOutOfScreen() override;
};

inline bool SplitEnemy::CheckOutOfScreen() {
	if (LONG(m_Texture->GetPosition().x + m_Texture->GetImageCenter().x) < GetWindowSize().left && LONG(m_SplitImage->GetPosition().x + m_SplitImage->GetImageCenter().x) < GetWindowSize().left) {
		return true;
	}
	return false;
}

inline void SplitEnemy::CalculateReflectDirection(class TextureClass* Texture, ImageOrder Order) {
	m_ReflectDirection[Order] = Texture->GetPosition() - m_BeforePosition[Order];
	D3DXVECTOR3 InverseDirection = m_BeforePosition[Order] - Texture->GetPosition();
	float Dot = D3DXVec3Dot(&InverseDirection, &m_NormalVector[Order]);

	m_ReflectDirection[Order] += (2 * m_NormalVector[Order] * Dot);
	D3DXVec3Normalize(&m_ReflectDirection[Order], &m_ReflectDirection[Order]);
	m_bIsReflect[Order] = true;
}

inline bool SplitEnemy::CheckHeight(class TextureClass* Texture, D3DXVECTOR3& Direction) {
	if (LONG(Texture->GetPosition().y + Texture->GetImageCenter().y) > GetWindowSize().bottom) {
		Direction = D3DXVECTOR3(0.f, -1.f, 0.f);
		return true;
	}
	else if (LONG(Texture->GetPosition().y - Texture->GetImageCenter().y) < GetWindowSize().top) {
		Direction = D3DXVECTOR3(0.f, 1.f, 0.f);
		return true;
	}
	return false;
}

inline void SplitEnemy::CheckSplitState(class TextureClass* Texture, ImageOrder Order) {
	m_BeforePosition[Order] = Texture->GetPosition();

	switch (m_States) {
	case SPLIT_NONE:
		Texture->AddXPosition(m_XMoveSpeed * -1);
		break;
	case SPLIT_SPLIT:
		if (m_bIsReflect[Order]) {
			Texture->AddPosition(D3DXVECTOR3(m_ReflectDirection[Order].x * m_XMoveSpeed * 2, m_ReflectDirection[Order].y * (m_YMoveSpeed * 2), 0.f));
		}
		else {
			Texture->AddPosition(D3DXVECTOR3(-5.f, (Order == FIRST ? 2.5f : -2.5f), 0.f));
		}
		break;
	}
}