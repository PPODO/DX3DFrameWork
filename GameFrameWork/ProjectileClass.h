#pragma once
#include "Actor.h"

class ProjectileClass : public Actor {
private:


protected:
	virtual void ProjectileMoveProcessing() = 0;

public:
	ProjectileClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void Destroy() override;

};

