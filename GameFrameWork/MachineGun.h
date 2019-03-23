#pragma once
#include "Weapon.h"

class MachineGun : public Weapon {
private:
	float m_MinRotation, m_MaxRotation;
	float m_CurrentRotation;
	float m_AngleScale;

	D3DXMATRIX m_Rotation, m_Default;
	D3DXMATRIX m_ZeroPoint, m_RotationAngle, m_Location;
	
	D3DXVECTOR3 m_Offset;

private:
	void CalculateWeaponAngle();

public:
	MachineGun();
	virtual ~MachineGun() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;

public:
	void SetRotation(float Value);

	inline D3DXVECTOR3 GetOffset() const { return m_Offset; }
	inline float GetCurrentAngle() const { return m_CurrentRotation; }

};