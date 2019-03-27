#pragma once
#include "Weapon.h"

class Cannon : public Weapon {
private:


protected:
	virtual void CalculateWeaponAngle() override;

public:
	Cannon();
	virtual ~Cannon() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;

};