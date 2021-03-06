#pragma once
#include "Weapon.h"

class MachineGun : public Weapon {
private:

private:
	virtual void CalculateWeaponAngle() override;

public:
	MachineGun();
	virtual ~MachineGun() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;

};