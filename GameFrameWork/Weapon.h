#pragma once
#include "Actor.h"

enum EWEAPONTYPE { EWT_MACHINEGUN, EWT_CANNON, EWT_COUNT };

class Weapon : public Actor {
private:
	D3DXMATRIX  m_Default;

protected:
	D3DXMATRIX m_Matrix;
	D3DXMATRIX m_CenterLocation, m_RotationAngle, m_Location;

protected:
	float m_MinRotation, m_MaxRotation;
	float m_CurrentRotation;
	float m_AngleScale;

protected:
	D3DXVECTOR3 m_Offset;

protected:
	virtual void CalculateWeaponAngle() = 0;

public:
	Weapon();
	virtual ~Weapon() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void ClearObject() override;

public:
	void SetRotation(float Value);
	void AddRotation(float Value);

	D3DXVECTOR3 GetOffsetByAngle(const float& Angle);
	inline D3DXVECTOR3 GetOffset() const { return m_Offset; }
	inline float GetCurrentAngle() const { return m_CurrentRotation; }

};