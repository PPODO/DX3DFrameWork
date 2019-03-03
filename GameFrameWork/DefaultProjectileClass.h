#pragma once
#include "ProjectileClass.h"

class DefaultProjectileClass : public ProjectileClass {
private:


protected:
	virtual void ProjectileMoveProcessing() override;

public:
	DefaultProjectileClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;

};